const payload = document.getElementById("payload").value;
const status = document.getElementById("status");

document.getElementById("duckyForm").addEventListener("submit", async function(e) {
    e.preventDefault();
    
    status.textContent = "Payload sending ...";

    try {
        const response = await fetch('/run', {
            method: POST,
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
                },
            body: "payload=" + encodeURIComponent(paydload)
        });    
        const result = await response.text();
        status.textContent = `${result}`;
    } catch {
        status.textContent = `Error: ${result}`;
    }
});

document.getElementById("download").addEventListener("click", function(e) {
    const filename = prompt("Enter filename", "payload.bin");

    if (filename) {
        var element = document.createElement('a');
        element.setAttribute('href', 'data:text/plain;charset=utf_8,' + encodeURIComponent(payload));
        element.setAttribute('download', filename);
        element.style.display = 'none';
        document.body.appendChild(element);
        element.click();
        document.body.removeChild(element);
    }
});
