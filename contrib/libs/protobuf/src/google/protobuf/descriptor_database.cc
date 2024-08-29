// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include "google/protobuf/descriptor_database.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "y_absl/container/btree_set.h"
#include "y_absl/strings/ascii.h"
#include "y_absl/strings/match.h"
#include "y_absl/strings/str_replace.h"
#include "y_absl/strings/string_view.h"
#include "google/protobuf/descriptor.pb.h"


namespace google {
namespace protobuf {

namespace {
void RecordMessageNames(const DescriptorProto& desc_proto,
                        y_absl::string_view prefix,
                        y_absl::btree_set<TProtoStringType>* output) {
  Y_ABSL_CHECK(desc_proto.has_name());
  TProtoStringType full_name = prefix.empty()
                              ? desc_proto.name()
                              : y_absl::StrCat(prefix, ".", desc_proto.name());
  output->insert(full_name);

  for (const auto& d : desc_proto.nested_type()) {
    RecordMessageNames(d, full_name, output);
  }
}

void RecordMessageNames(const FileDescriptorProto& file_proto,
                        y_absl::btree_set<TProtoStringType>* output) {
  for (const auto& d : file_proto.message_type()) {
    RecordMessageNames(d, file_proto.package(), output);
  }
}

template <typename Fn>
bool ForAllFileProtos(DescriptorDatabase* db, Fn callback,
                      std::vector<TProtoStringType>* output) {
  std::vector<TProtoStringType> file_names;
  if (!db->FindAllFileNames(&file_names)) {
    return false;
  }
  y_absl::btree_set<TProtoStringType> set;
  FileDescriptorProto file_proto;
  for (const auto& f : file_names) {
    file_proto.Clear();
    if (!db->FindFileByName(f, &file_proto)) {
      Y_ABSL_LOG(ERROR) << "File not found in database (unexpected): " << f;
      return false;
    }
    callback(file_proto, &set);
  }
  output->insert(output->end(), set.begin(), set.end());
  return true;
}
}  // namespace

DescriptorDatabase::~DescriptorDatabase() = default;

bool DescriptorDatabase::FindAllPackageNames(std::vector<TProtoStringType>* output) {
  return ForAllFileProtos(
      this,
      [](const FileDescriptorProto& file_proto,
         y_absl::btree_set<TProtoStringType>* set) {
        set->insert(file_proto.package());
      },
      output);
}

bool DescriptorDatabase::FindAllMessageNames(std::vector<TProtoStringType>* output) {
  return ForAllFileProtos(
      this,
      [](const FileDescriptorProto& file_proto,
         y_absl::btree_set<TProtoStringType>* set) {
        RecordMessageNames(file_proto, set);
      },
      output);
}

// ===================================================================

SimpleDescriptorDatabase::SimpleDescriptorDatabase() {}
SimpleDescriptorDatabase::~SimpleDescriptorDatabase() {}

template <typename Value>
bool SimpleDescriptorDatabase::DescriptorIndex<Value>::AddFile(
    const FileDescriptorProto& file, Value value) {
  if (!by_name_.emplace(file.name(), value).second) {
    Y_ABSL_LOG(ERROR) << "File already exists in database: " << file.name();
    return false;
  }

  // We must be careful here -- calling file.package() if file.has_package() is
  // false could access an uninitialized static-storage variable if we are being
  // run at startup time.
  TProtoStringType path = file.has_package() ? file.package() : TProtoStringType();
  if (!path.empty()) path += '.';

  for (int i = 0; i < file.message_type_size(); i++) {
    if (!AddSymbol(path + file.message_type(i).name(), value)) return false;
    if (!AddNestedExtensions(file.name(), file.message_type(i), value))
      return false;
  }
  for (int i = 0; i < file.enum_type_size(); i++) {
    if (!AddSymbol(path + file.enum_type(i).name(), value)) return false;
  }
  for (int i = 0; i < file.extension_size(); i++) {
    if (!AddSymbol(path + file.extension(i).name(), value)) return false;
    if (!AddExtension(file.name(), file.extension(i), value)) return false;
  }
  for (int i = 0; i < file.service_size(); i++) {
    if (!AddSymbol(path + file.service(i).name(), value)) return false;
  }

  return true;
}

namespace {

// Returns true if and only if all characters in the name are alphanumerics,
// underscores, or periods.
bool ValidateSymbolName(y_absl::string_view name) {
  for (char c : name) {
    // I don't trust ctype.h due to locales.  :(
    if (c != '.' && c != '_' && (c < '0' || c > '9') && (c < 'A' || c > 'Z') &&
        (c < 'a' || c > 'z')) {
      return false;
    }
  }
  return true;
}

// Find the last key in the container which sorts less than or equal to the
// symbol name.  Since upper_bound() returns the *first* key that sorts
// *greater* than the input, we want the element immediately before that.
template <typename Container, typename Key>
typename Container::const_iterator FindLastLessOrEqual(
    const Container* container, const Key& key) {
  auto iter = container->upper_bound(key);
  if (iter != container->begin()) --iter;
  return iter;
}

// As above, but using std::upper_bound instead.
template <typename Container, typename Key, typename Cmp>
typename Container::const_iterator FindLastLessOrEqual(
    const Container* container, const Key& key, const Cmp& cmp) {
  auto iter = std::upper_bound(container->begin(), container->end(), key, cmp);
  if (iter != container->begin()) --iter;
  return iter;
}

// True if either the arguments are equal or super_symbol identifies a
// parent symbol of sub_symbol (e.g. "foo.bar" is a parent of
// "foo.bar.baz", but not a parent of "foo.barbaz").
bool IsSubSymbol(y_absl::string_view sub_symbol, y_absl::string_view super_symbol) {
  return sub_symbol == super_symbol ||
         (y_absl::StartsWith(super_symbol, sub_symbol) &&
          super_symbol[sub_symbol.size()] == '.');
}

}  // namespace

template <typename Value>
bool SimpleDescriptorDatabase::DescriptorIndex<Value>::AddSymbol(
    y_absl::string_view name, Value value) {
  // We need to make sure not to violate our map invariant.

  // If the symbol name is invalid it could break our lookup algorithm (which
  // relies on the fact that '.' sorts before all other characters that are
  // valid in symbol names).
  if (!ValidateSymbolName(name)) {
    Y_ABSL_LOG(ERROR) << "Invalid symbol name: " << name;
    return false;
  }

  // Try to look up the symbol to make sure a super-symbol doesn't already
  // exist.
  auto iter = FindLastLessOrEqual(&by_symbol_, name);

  if (iter == by_symbol_.end()) {
    // Apparently the map is currently empty.  Just insert and be done with it.
    by_symbol_.try_emplace(name, value);
    return true;
  }

  if (IsSubSymbol(iter->first, name)) {
    Y_ABSL_LOG(ERROR) << "Symbol name \"" << name
                    << "\" conflicts with the existing "
                       "symbol \""
                    << iter->first << "\".";
    return false;
  }

  // OK, that worked.  Now we have to make sure that no symbol in the map is
  // a sub-symbol of the one we are inserting.  The only symbol which could
  // be so is the first symbol that is greater than the new symbol.  Since
  // |iter| points at the last symbol that is less than or equal, we just have
  // to increment it.
  ++iter;

  if (iter != by_symbol_.end() && IsSubSymbol(name, iter->first)) {
    Y_ABSL_LOG(ERROR) << "Symbol name \"" << name
                    << "\" conflicts with the existing "
                       "symbol \""
                    << iter->first << "\".";
    return false;
  }

  // OK, no conflicts.

  // Insert the new symbol using the iterator as a hint, the new entry will
  // appear immediately before the one the iterator is pointing at.
  by_symbol_.insert(iter, {TProtoStringType(name), value});

  return true;
}

template <typename Value>
bool SimpleDescriptorDatabase::DescriptorIndex<Value>::AddNestedExtensions(
    const TProtoStringType& filename, const DescriptorProto& message_type,
    Value value) {
  for (int i = 0; i < message_type.nested_type_size(); i++) {
    if (!AddNestedExtensions(filename, message_type.nested_type(i), value))
      return false;
  }
  for (int i = 0; i < message_type.extension_size(); i++) {
    if (!AddExtension(filename, message_type.extension(i), value)) return false;
  }
  return true;
}

template <typename Value>
bool SimpleDescriptorDatabase::DescriptorIndex<Value>::AddExtension(
    const TProtoStringType& filename, const FieldDescriptorProto& field,
    Value value) {
  if (!field.extendee().empty() && field.extendee()[0] == '.') {
    // The extension is fully-qualified.  We can use it as a lookup key in
    // the by_symbol_ table.
    if (!by_extension_
             .emplace(
                 std::make_pair(field.extendee().substr(1), field.number()),
                 value)
             .second) {
      Y_ABSL_LOG(ERROR)
          << "Extension conflicts with extension already in database: "
             "extend "
          << field.extendee() << " { " << field.name() << " = "
          << field.number() << " } from:" << filename;
      return false;
    }
  } else {
    // Not fully-qualified.  We can't really do anything here, unfortunately.
    // We don't consider this an error, though, because the descriptor is
    // valid.
  }
  return true;
}

template <typename Value>
Value SimpleDescriptorDatabase::DescriptorIndex<Value>::FindFile(
    const TProtoStringType& filename) {
  auto it = by_name_.find(filename);
  if (it == by_name_.end()) return {};
  return it->second;
}

template <typename Value>
Value SimpleDescriptorDatabase::DescriptorIndex<Value>::FindSymbol(
    const TProtoStringType& name) {
  auto iter = FindLastLessOrEqual(&by_symbol_, name);

  return (iter != by_symbol_.end() && IsSubSymbol(iter->first, name))
             ? iter->second
             : Value();
}

template <typename Value>
Value SimpleDescriptorDatabase::DescriptorIndex<Value>::FindExtension(
    const TProtoStringType& containing_type, int field_number) {
  auto it = by_extension_.find({containing_type, field_number});
  if (it == by_extension_.end()) return {};
  return it->second;
}

template <typename Value>
bool SimpleDescriptorDatabase::DescriptorIndex<Value>::FindAllExtensionNumbers(
    const TProtoStringType& containing_type, std::vector<int>* output) {
  auto it = by_extension_.lower_bound(std::make_pair(containing_type, 0));
  bool success = false;

  for (; it != by_extension_.end() && it->first.first == containing_type;
       ++it) {
    output->push_back(it->first.second);
    success = true;
  }

  return success;
}

template <typename Value>
void SimpleDescriptorDatabase::DescriptorIndex<Value>::FindAllFileNames(
    std::vector<TProtoStringType>* output) {
  output->resize(by_name_.size());
  int i = 0;
  for (const auto& kv : by_name_) {
    (*output)[i] = kv.first;
    i++;
  }
}

// -------------------------------------------------------------------

bool SimpleDescriptorDatabase::Add(const FileDescriptorProto& file) {
  FileDescriptorProto* new_file = new FileDescriptorProto;
  new_file->CopyFrom(file);
  return AddAndOwn(new_file);
}

bool SimpleDescriptorDatabase::AddAndOwn(const FileDescriptorProto* file) {
  files_to_delete_.emplace_back(file);
  return index_.AddFile(*file, file);
}

bool SimpleDescriptorDatabase::AddUnowned(const FileDescriptorProto* file) {
  return index_.AddFile(*file, file);
}

bool SimpleDescriptorDatabase::FindFileByName(const TProtoStringType& filename,
                                              FileDescriptorProto* output) {
  return MaybeCopy(index_.FindFile(filename), output);
}

bool SimpleDescriptorDatabase::FindFileContainingSymbol(
    const TProtoStringType& symbol_name, FileDescriptorProto* output) {
  return MaybeCopy(index_.FindSymbol(symbol_name), output);
}

bool SimpleDescriptorDatabase::FindFileContainingExtension(
    const TProtoStringType& containing_type, int field_number,
    FileDescriptorProto* output) {
  return MaybeCopy(index_.FindExtension(containing_type, field_number), output);
}

bool SimpleDescriptorDatabase::FindAllExtensionNumbers(
    const TProtoStringType& extendee_type, std::vector<int>* output) {
  return index_.FindAllExtensionNumbers(extendee_type, output);
}


bool SimpleDescriptorDatabase::FindAllFileNames(
    std::vector<TProtoStringType>* output) {
  index_.FindAllFileNames(output);
  return true;
}

bool SimpleDescriptorDatabase::MaybeCopy(const FileDescriptorProto* file,
                                         FileDescriptorProto* output) {
  if (file == nullptr) return false;
  output->CopyFrom(*file);
  return true;
}

// -------------------------------------------------------------------

class EncodedDescriptorDatabase::DescriptorIndex {
 public:
  using Value = std::pair<const void*, int>;
  // Helpers to recursively add particular descriptors and all their contents
  // to the index.
  template <typename FileProto>
  bool AddFile(const FileProto& file, Value value);

  Value FindFile(y_absl::string_view filename);
  Value FindSymbol(y_absl::string_view name);
  Value FindSymbolOnlyFlat(y_absl::string_view name) const;
  Value FindExtension(y_absl::string_view containing_type, int field_number);
  bool FindAllExtensionNumbers(y_absl::string_view containing_type,
                               std::vector<int>* output);
  void FindAllFileNames(std::vector<TProtoStringType>* output) const;

 private:
  friend class EncodedDescriptorDatabase;

  bool AddSymbol(y_absl::string_view symbol);

  template <typename DescProto>
  bool AddNestedExtensions(y_absl::string_view filename,
                           const DescProto& message_type);
  template <typename FieldProto>
  bool AddExtension(y_absl::string_view filename, const FieldProto& field);

  // All the maps below have two representations:
  //  - a y_absl::btree_set<> where we insert initially.
  //  - a std::vector<> where we flatten the structure on demand.
  // The initial tree helps avoid O(N) behavior of inserting into a sorted
  // vector, while the vector reduces the heap requirements of the data
  // structure.

  void EnsureFlat();

  using String = TProtoStringType;

  String EncodeString(y_absl::string_view str) const { return String(str); }
  y_absl::string_view DecodeString(const String& str, int) const { return str; }

  struct EncodedEntry {
    // Do not use `Value` here to avoid the padding of that object.
    const void* data;
    int size;
    // Keep the package here instead of each SymbolEntry to save space.
    String encoded_package;

    Value value() const { return {data, size}; }
  };
  std::vector<EncodedEntry> all_values_;

  struct FileEntry {
    int data_offset;
    String encoded_name;

    y_absl::string_view name(const DescriptorIndex& index) const {
      return index.DecodeString(encoded_name, data_offset);
    }
  };
  struct FileCompare {
    const DescriptorIndex& index;

    bool operator()(const FileEntry& a, const FileEntry& b) const {
      return a.name(index) < b.name(index);
    }
    bool operator()(const FileEntry& a, y_absl::string_view b) const {
      return a.name(index) < b;
    }
    bool operator()(y_absl::string_view a, const FileEntry& b) const {
      return a < b.name(index);
    }
  };
  y_absl::btree_set<FileEntry, FileCompare> by_name_{FileCompare{*this}};
  std::vector<FileEntry> by_name_flat_;

  struct SymbolEntry {
    int data_offset;
    String encoded_symbol;

    y_absl::string_view package(const DescriptorIndex& index) const {
      return index.DecodeString(index.all_values_[data_offset].encoded_package,
                                data_offset);
    }
    y_absl::string_view symbol(const DescriptorIndex& index) const {
      return index.DecodeString(encoded_symbol, data_offset);
    }

    TProtoStringType AsString(const DescriptorIndex& index) const {
      auto p = package(index);
      return y_absl::StrCat(p, p.empty() ? "" : ".", symbol(index));
    }
  };

  struct SymbolCompare {
    const DescriptorIndex& index;

    TProtoStringType AsString(const SymbolEntry& entry) const {
      return entry.AsString(index);
    }
    static y_absl::string_view AsString(y_absl::string_view str) { return str; }

    std::pair<y_absl::string_view, y_absl::string_view> GetParts(
        const SymbolEntry& entry) const {
      auto package = entry.package(index);
      if (package.empty()) return {entry.symbol(index), y_absl::string_view{}};
      return {package, entry.symbol(index)};
    }
    std::pair<y_absl::string_view, y_absl::string_view> GetParts(
        y_absl::string_view str) const {
      return {str, {}};
    }

    template <typename T, typename U>
    bool operator()(const T& lhs, const U& rhs) const {
      auto lhs_parts = GetParts(lhs);
      auto rhs_parts = GetParts(rhs);

      // Fast path to avoid making the whole string for common cases.
      if (int res =
              lhs_parts.first.substr(0, rhs_parts.first.size())
                  .compare(rhs_parts.first.substr(0, lhs_parts.first.size()))) {
        // If the packages already differ, exit early.
        return res < 0;
      } else if (lhs_parts.first.size() == rhs_parts.first.size()) {
        return lhs_parts.second < rhs_parts.second;
      }
      return AsString(lhs) < AsString(rhs);
    }
  };
  y_absl::btree_set<SymbolEntry, SymbolCompare> by_symbol_{SymbolCompare{*this}};
  std::vector<SymbolEntry> by_symbol_flat_;

  struct ExtensionEntry {
    int data_offset;
    String encoded_extendee;
    y_absl::string_view extendee(const DescriptorIndex& index) const {
      return index.DecodeString(encoded_extendee, data_offset).substr(1);
    }
    int extension_number;
  };
  struct ExtensionCompare {
    const DescriptorIndex& index;

    bool operator()(const ExtensionEntry& a, const ExtensionEntry& b) const {
      return std::make_tuple(a.extendee(index), a.extension_number) <
             std::make_tuple(b.extendee(index), b.extension_number);
    }
    bool operator()(const ExtensionEntry& a,
                    std::tuple<y_absl::string_view, int> b) const {
      return std::make_tuple(a.extendee(index), a.extension_number) < b;
    }
    bool operator()(std::tuple<y_absl::string_view, int> a,
                    const ExtensionEntry& b) const {
      return a < std::make_tuple(b.extendee(index), b.extension_number);
    }
  };
  y_absl::btree_set<ExtensionEntry, ExtensionCompare> by_extension_{
      ExtensionCompare{*this}};
  std::vector<ExtensionEntry> by_extension_flat_;
};

bool EncodedDescriptorDatabase::Add(const void* encoded_file_descriptor,
                                    int size) {
  FileDescriptorProto file;
  if (file.ParseFromArray(encoded_file_descriptor, size)) {
    return index_->AddFile(file, std::make_pair(encoded_file_descriptor, size));
  } else {
    Y_ABSL_LOG(ERROR) << "Invalid file descriptor data passed to "
                       "EncodedDescriptorDatabase::Add().";
    return false;
  }
}

bool EncodedDescriptorDatabase::AddCopy(const void* encoded_file_descriptor,
                                        int size) {
  void* copy = operator new(size);
  memcpy(copy, encoded_file_descriptor, size);
  files_to_delete_.push_back(copy);
  return Add(copy, size);
}

bool EncodedDescriptorDatabase::FindFileByName(const TProtoStringType& filename,
                                               FileDescriptorProto* output) {
  return MaybeParse(index_->FindFile(filename), output);
}

bool EncodedDescriptorDatabase::FindFileContainingSymbol(
    const TProtoStringType& symbol_name, FileDescriptorProto* output) {
  return MaybeParse(index_->FindSymbol(symbol_name), output);
}

bool EncodedDescriptorDatabase::FindNameOfFileContainingSymbol(
    const TProtoStringType& symbol_name, TProtoStringType* output) {
  auto encoded_file = index_->FindSymbol(symbol_name);
  if (encoded_file.first == nullptr) return false;

  // Optimization:  The name should be the first field in the encoded message.
  //   Try to just read it directly.
  io::CodedInputStream input(static_cast<const uint8_t*>(encoded_file.first),
                             encoded_file.second);

  const arc_ui32 kNameTag = internal::WireFormatLite::MakeTag(
      FileDescriptorProto::kNameFieldNumber,
      internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED);

  if (input.ReadTagNoLastTag() == kNameTag) {
    // Success!
    return internal::WireFormatLite::ReadString(&input, output);
  } else {
    // Slow path.  Parse whole message.
    FileDescriptorProto file_proto;
    if (!file_proto.ParseFromArray(encoded_file.first, encoded_file.second)) {
      return false;
    }
    *output = file_proto.name();
    return true;
  }
}

bool EncodedDescriptorDatabase::FindFileContainingExtension(
    const TProtoStringType& containing_type, int field_number,
    FileDescriptorProto* output) {
  return MaybeParse(index_->FindExtension(containing_type, field_number),
                    output);
}

bool EncodedDescriptorDatabase::FindAllExtensionNumbers(
    const TProtoStringType& extendee_type, std::vector<int>* output) {
  return index_->FindAllExtensionNumbers(extendee_type, output);
}

template <typename FileProto>
bool EncodedDescriptorDatabase::DescriptorIndex::AddFile(const FileProto& file,
                                                         Value value) {
  // We push `value` into the array first. This is important because the AddXXX
  // functions below will expect it to be there.
  all_values_.push_back({value.first, value.second, {}});

  if (!ValidateSymbolName(file.package())) {
    Y_ABSL_LOG(ERROR) << "Invalid package name: " << file.package();
    return false;
  }
  all_values_.back().encoded_package = EncodeString(file.package());

  if (!by_name_
           .insert({static_cast<int>(all_values_.size() - 1),
                    EncodeString(file.name())})
           .second ||
      std::binary_search(by_name_flat_.begin(), by_name_flat_.end(),
                         file.name(), by_name_.key_comp())) {
    Y_ABSL_LOG(ERROR) << "File already exists in database: " << file.name();
    return false;
  }

  for (const auto& message_type : file.message_type()) {
    if (!AddSymbol(message_type.name())) return false;
    if (!AddNestedExtensions(file.name(), message_type)) return false;
  }
  for (const auto& enum_type : file.enum_type()) {
    if (!AddSymbol(enum_type.name())) return false;
  }
  for (const auto& extension : file.extension()) {
    if (!AddSymbol(extension.name())) return false;
    if (!AddExtension(file.name(), extension)) return false;
  }
  for (const auto& service : file.service()) {
    if (!AddSymbol(service.name())) return false;
  }

  return true;
}

template <typename Iter, typename Iter2, typename Index>
static bool CheckForMutualSubsymbols(y_absl::string_view symbol_name, Iter* iter,
                                     Iter2 end, const Index& index) {
  if (*iter != end) {
    if (IsSubSymbol((*iter)->AsString(index), symbol_name)) {
      Y_ABSL_LOG(ERROR) << "Symbol name \"" << symbol_name
                      << "\" conflicts with the existing symbol \""
                      << (*iter)->AsString(index) << "\".";
      return false;
    }

    // OK, that worked.  Now we have to make sure that no symbol in the map is
    // a sub-symbol of the one we are inserting.  The only symbol which could
    // be so is the first symbol that is greater than the new symbol.  Since
    // |iter| points at the last symbol that is less than or equal, we just have
    // to increment it.
    ++*iter;

    if (*iter != end && IsSubSymbol(symbol_name, (*iter)->AsString(index))) {
      Y_ABSL_LOG(ERROR) << "Symbol name \"" << symbol_name
                      << "\" conflicts with the existing symbol \""
                      << (*iter)->AsString(index) << "\".";
      return false;
    }
  }
  return true;
}

bool EncodedDescriptorDatabase::DescriptorIndex::AddSymbol(
    y_absl::string_view symbol) {
  SymbolEntry entry = {static_cast<int>(all_values_.size() - 1),
                       EncodeString(symbol)};
  TProtoStringType entry_as_string = entry.AsString(*this);

  // We need to make sure not to violate our map invariant.

  // If the symbol name is invalid it could break our lookup algorithm (which
  // relies on the fact that '.' sorts before all other characters that are
  // valid in symbol names).
  if (!ValidateSymbolName(symbol)) {
    Y_ABSL_LOG(ERROR) << "Invalid symbol name: " << entry_as_string;
    return false;
  }

  auto iter = FindLastLessOrEqual(&by_symbol_, entry);
  if (!CheckForMutualSubsymbols(entry_as_string, &iter, by_symbol_.end(),
                                *this)) {
    return false;
  }

  // Same, but on by_symbol_flat_
  auto flat_iter =
      FindLastLessOrEqual(&by_symbol_flat_, entry, by_symbol_.key_comp());
  if (!CheckForMutualSubsymbols(entry_as_string, &flat_iter,
                                by_symbol_flat_.end(), *this)) {
    return false;
  }

  // OK, no conflicts.

  // Insert the new symbol using the iterator as a hint, the new entry will
  // appear immediately before the one the iterator is pointing at.
  by_symbol_.insert(iter, entry);

  return true;
}

template <typename DescProto>
bool EncodedDescriptorDatabase::DescriptorIndex::AddNestedExtensions(
    y_absl::string_view filename, const DescProto& message_type) {
  for (const auto& nested_type : message_type.nested_type()) {
    if (!AddNestedExtensions(filename, nested_type)) return false;
  }
  for (const auto& extension : message_type.extension()) {
    if (!AddExtension(filename, extension)) return false;
  }
  return true;
}

template <typename FieldProto>
bool EncodedDescriptorDatabase::DescriptorIndex::AddExtension(
    y_absl::string_view filename, const FieldProto& field) {
  if (!field.extendee().empty() && field.extendee()[0] == '.') {
    // The extension is fully-qualified.  We can use it as a lookup key in
    // the by_symbol_ table.
    if (!by_extension_
             .insert({static_cast<int>(all_values_.size() - 1),
                      EncodeString(field.extendee()), field.number()})
             .second ||
        std::binary_search(
            by_extension_flat_.begin(), by_extension_flat_.end(),
            std::make_pair(field.extendee().substr(1), field.number()),
            by_extension_.key_comp())) {
      Y_ABSL_LOG(ERROR)
          << "Extension conflicts with extension already in database: "
             "extend "
          << field.extendee() << " { " << field.name() << " = "
          << field.number() << " } from:" << filename;
      return false;
    }
  } else {
    // Not fully-qualified.  We can't really do anything here, unfortunately.
    // We don't consider this an error, though, because the descriptor is
    // valid.
  }
  return true;
}

std::pair<const void*, int>
EncodedDescriptorDatabase::DescriptorIndex::FindSymbol(y_absl::string_view name) {
  EnsureFlat();
  return FindSymbolOnlyFlat(name);
}

std::pair<const void*, int>
EncodedDescriptorDatabase::DescriptorIndex::FindSymbolOnlyFlat(
    y_absl::string_view name) const {
  auto iter =
      FindLastLessOrEqual(&by_symbol_flat_, name, by_symbol_.key_comp());

  return iter != by_symbol_flat_.end() &&
                 IsSubSymbol(iter->AsString(*this), name)
             ? all_values_[iter->data_offset].value()
             : Value();
}

std::pair<const void*, int>
EncodedDescriptorDatabase::DescriptorIndex::FindExtension(
    y_absl::string_view containing_type, int field_number) {
  EnsureFlat();

  auto it = std::lower_bound(
      by_extension_flat_.begin(), by_extension_flat_.end(),
      std::make_tuple(containing_type, field_number), by_extension_.key_comp());
  return it == by_extension_flat_.end() ||
                 it->extendee(*this) != containing_type ||
                 it->extension_number != field_number
             ? std::make_pair(nullptr, 0)
             : all_values_[it->data_offset].value();
}

template <typename T, typename Less>
static void MergeIntoFlat(y_absl::btree_set<T, Less>* s, std::vector<T>* flat) {
  if (s->empty()) return;
  std::vector<T> new_flat(s->size() + flat->size());
  std::merge(s->begin(), s->end(), flat->begin(), flat->end(), &new_flat[0],
             s->key_comp());
  *flat = std::move(new_flat);
  s->clear();
}

void EncodedDescriptorDatabase::DescriptorIndex::EnsureFlat() {
  all_values_.shrink_to_fit();
  // Merge each of the sets into their flat counterpart.
  MergeIntoFlat(&by_name_, &by_name_flat_);
  MergeIntoFlat(&by_symbol_, &by_symbol_flat_);
  MergeIntoFlat(&by_extension_, &by_extension_flat_);
}

bool EncodedDescriptorDatabase::DescriptorIndex::FindAllExtensionNumbers(
    y_absl::string_view containing_type, std::vector<int>* output) {
  EnsureFlat();

  bool success = false;
  auto it = std::lower_bound(
      by_extension_flat_.begin(), by_extension_flat_.end(),
      std::make_tuple(containing_type, 0), by_extension_.key_comp());
  for (;
       it != by_extension_flat_.end() && it->extendee(*this) == containing_type;
       ++it) {
    output->push_back(it->extension_number);
    success = true;
  }

  return success;
}

void EncodedDescriptorDatabase::DescriptorIndex::FindAllFileNames(
    std::vector<TProtoStringType>* output) const {
  output->resize(by_name_.size() + by_name_flat_.size());
  int i = 0;
  for (const auto& entry : by_name_) {
    (*output)[i] = TProtoStringType(entry.name(*this));
    i++;
  }
  for (const auto& entry : by_name_flat_) {
    (*output)[i] = TProtoStringType(entry.name(*this));
    i++;
  }
}

std::pair<const void*, int>
EncodedDescriptorDatabase::DescriptorIndex::FindFile(
    y_absl::string_view filename) {
  EnsureFlat();

  auto it = std::lower_bound(by_name_flat_.begin(), by_name_flat_.end(),
                             filename, by_name_.key_comp());
  return it == by_name_flat_.end() || it->name(*this) != filename
             ? std::make_pair(nullptr, 0)
             : all_values_[it->data_offset].value();
}


bool EncodedDescriptorDatabase::FindAllFileNames(
    std::vector<TProtoStringType>* output) {
  index_->FindAllFileNames(output);
  return true;
}

bool EncodedDescriptorDatabase::MaybeParse(
    std::pair<const void*, int> encoded_file, FileDescriptorProto* output) {
  if (encoded_file.first == nullptr) return false;
  return output->ParseFromArray(encoded_file.first, encoded_file.second);
}

EncodedDescriptorDatabase::EncodedDescriptorDatabase()
    : index_(new DescriptorIndex()) {}

EncodedDescriptorDatabase::~EncodedDescriptorDatabase() {
  for (void* p : files_to_delete_) {
    operator delete(p);
  }
}

// ===================================================================

DescriptorPoolDatabase::DescriptorPoolDatabase(const DescriptorPool& pool)
    : pool_(pool) {}
DescriptorPoolDatabase::~DescriptorPoolDatabase() {}

bool DescriptorPoolDatabase::FindFileByName(const TProtoStringType& filename,
                                            FileDescriptorProto* output) {
  const FileDescriptor* file = pool_.FindFileByName(filename);
  if (file == nullptr) return false;
  output->Clear();
  file->CopyTo(output);
  return true;
}

bool DescriptorPoolDatabase::FindFileContainingSymbol(
    const TProtoStringType& symbol_name, FileDescriptorProto* output) {
  const FileDescriptor* file = pool_.FindFileContainingSymbol(symbol_name);
  if (file == nullptr) return false;
  output->Clear();
  file->CopyTo(output);
  return true;
}

bool DescriptorPoolDatabase::FindFileContainingExtension(
    const TProtoStringType& containing_type, int field_number,
    FileDescriptorProto* output) {
  const Descriptor* extendee = pool_.FindMessageTypeByName(containing_type);
  if (extendee == nullptr) return false;

  const FieldDescriptor* extension =
      pool_.FindExtensionByNumber(extendee, field_number);
  if (extension == nullptr) return false;

  output->Clear();
  extension->file()->CopyTo(output);
  return true;
}

bool DescriptorPoolDatabase::FindAllExtensionNumbers(
    const TProtoStringType& extendee_type, std::vector<int>* output) {
  const Descriptor* extendee = pool_.FindMessageTypeByName(extendee_type);
  if (extendee == nullptr) return false;

  std::vector<const FieldDescriptor*> extensions;
  pool_.FindAllExtensions(extendee, &extensions);

  for (const FieldDescriptor* extension : extensions) {
    output->push_back(extension->number());
  }

  return true;
}

// ===================================================================

MergedDescriptorDatabase::MergedDescriptorDatabase(
    DescriptorDatabase* source1, DescriptorDatabase* source2) {
  sources_.push_back(source1);
  sources_.push_back(source2);
}
MergedDescriptorDatabase::MergedDescriptorDatabase(
    const std::vector<DescriptorDatabase*>& sources)
    : sources_(sources) {}
MergedDescriptorDatabase::~MergedDescriptorDatabase() {}

bool MergedDescriptorDatabase::FindFileByName(const TProtoStringType& filename,
                                              FileDescriptorProto* output) {
  for (DescriptorDatabase* source : sources_) {
    if (source->FindFileByName(filename, output)) {
      return true;
    }
  }
  return false;
}

bool MergedDescriptorDatabase::FindFileContainingSymbol(
    const TProtoStringType& symbol_name, FileDescriptorProto* output) {
  for (size_t i = 0; i < sources_.size(); i++) {
    if (sources_[i]->FindFileContainingSymbol(symbol_name, output)) {
      // The symbol was found in source i.  However, if one of the previous
      // sources defines a file with the same name (which presumably doesn't
      // contain the symbol, since it wasn't found in that source), then we
      // must hide it from the caller.
      FileDescriptorProto temp;
      for (size_t j = 0; j < i; j++) {
        if (sources_[j]->FindFileByName(output->name(), &temp)) {
          // Found conflicting file in a previous source.
          return false;
        }
      }
      return true;
    }
  }
  return false;
}

bool MergedDescriptorDatabase::FindFileContainingExtension(
    const TProtoStringType& containing_type, int field_number,
    FileDescriptorProto* output) {
  for (size_t i = 0; i < sources_.size(); i++) {
    if (sources_[i]->FindFileContainingExtension(containing_type, field_number,
                                                 output)) {
      // The symbol was found in source i.  However, if one of the previous
      // sources defines a file with the same name (which presumably doesn't
      // contain the symbol, since it wasn't found in that source), then we
      // must hide it from the caller.
      FileDescriptorProto temp;
      for (size_t j = 0; j < i; j++) {
        if (sources_[j]->FindFileByName(output->name(), &temp)) {
          // Found conflicting file in a previous source.
          return false;
        }
      }
      return true;
    }
  }
  return false;
}

bool MergedDescriptorDatabase::FindAllExtensionNumbers(
    const TProtoStringType& extendee_type, std::vector<int>* output) {
  y_absl::btree_set<int> merged_results;
  std::vector<int> results;
  bool success = false;

  for (DescriptorDatabase* source : sources_) {
    if (source->FindAllExtensionNumbers(extendee_type, &results)) {
      std::copy(results.begin(), results.end(),
                std::insert_iterator<y_absl::btree_set<int> >(
                    merged_results, merged_results.begin()));
      success = true;
    }
    results.clear();
  }

  std::copy(merged_results.begin(), merged_results.end(),
            std::insert_iterator<std::vector<int> >(*output, output->end()));

  return success;
}


bool MergedDescriptorDatabase::FindAllFileNames(
    std::vector<TProtoStringType>* output) {
  bool implemented = false;
  for (DescriptorDatabase* source : sources_) {
    std::vector<TProtoStringType> source_output;
    if (source->FindAllFileNames(&source_output)) {
      output->reserve(output->size() + source_output.size());
      for (auto& source : source_output) {
        output->push_back(std::move(source));
      }
      implemented = true;
    }
  }
  return implemented;
}

}  // namespace protobuf
}  // namespace google
