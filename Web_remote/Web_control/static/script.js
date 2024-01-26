// web_control/static/script.js
function sendCommand(command) {
    var xhr = new XMLHttpRequest();
    xhr.open('POST', '/control', true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.send(JSON.stringify({ command: command }));
}
