const childProcess = require('child_process');
const fs = require("fs");

process.chdir('./build_scripts');

function compileBuildScripts() {
    const buildRun = childProcess.exec('../node_modules/.bin/tsc');
    
    return new Promise(resolve => {
        buildRun.on('error', err => resolve({err}));
        buildRun.on('exit', (code, signal) => resolve({code, signal}));
    });
}

function runScript(script) {
    process.chdir('..');

    const child = childProcess.fork('./build_scripts/out/' + script + '.js');
    return new Promise(resolve => {
        child.on('error', err => resolve({err}));
        child.on('exit', (code, signal) => resolve({code, signal}));
    });
}

function runCommand(command) {
    if (command === 'bootstrap') {
        compileBuildScripts().then(console.log);
        return;
    }

    runScript(command);
} 

const command = process.argv[process.argv.length - 1];

if (!fs.existsSync('./out')) {
    compileBuildScripts().then(result => {
        if (result.code === 0) {
            runCommand(command);
            return;
        }
        console.error(result);
    });
} else {
    runCommand(command);
}

