import { updateStatus } from "./editor.js";

document
  .getElementById("duckyForm")
  .addEventListener("submit", async function (e) {
    e.preventDefault();
    run();
  });

document.getElementById("download").addEventListener("click", function () {
  download();
});

document.getElementById("upload").addEventListener("click", function () {
  upload();
});

let statusInterval = null;

export async function run() {
  const runBtn = document.getElementById("runBtn");
  const payload = document.getElementById("payload").value;

  if (payload != "") {
    try {
      startLoadingStatus("Payload sending");
      runBtn.disabled = true;
      const response = await fetch("/run", {
        method: "POST",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: "script=" + encodeURIComponent(payload),
      });
      const result = await response.text();
      stopLoadingStatus();
      updateStatus(`${result}`);
    } catch (error) {
      stopLoadingStatus();
      updateStatus(`Error: ${error.message}`);
    } finally {
      runBtn.disabled = false;
    }
  } else {
    updateStatus("Error: Cannot send an empty payload");
  }
}

export function download() {
  const payload = document.getElementById("payload").value;

  if (payload != "") {
    const filename = prompt("Enter filename", "payload.bin");

    if (filename) {
      var element = document.createElement("a");
      element.setAttribute(
        "href",
        "data:text/plain;charset=utf_8," + encodeURIComponent(payload),
      );
      element.setAttribute("download", filename);
      element.style.display = "none";
      document.body.appendChild(element);
      element.click();
      document.body.removeChild(element);
      updateStatus("File downloaded correctly");
    }
  } else {
    updateStatus("Error: Cannot download an empty file");
  }
}

function upload() {
  const fileInput = document.createElement("input");
  fileInput.type = "file";
  fileInput.style.display = "none";
  document.body.appendChild(fileInput);

  fileInput.click();

  fileInput.addEventListener("change", function (e) {
    const payload = document.getElementById("payload");

    const file = e.target.files[0];

    if (!file) {
      console.error("Select a file please");
      fileInput.remove();
      return;
    }

    const maxSizeUpload = 50;
    if (file.size < maxSizeUpload * 1024) {
      updateStatus(`File: ${file.name} (${file.size} bytes) has been uploaded`);

      const reader = new FileReader();
      reader.onload = function (event) {
        try {
          const content = event.target.result;
          payload.value = content;
        } catch (error) {
          updateStatus(`Error: ${error.message}`);
          console.error("Error: " + error.message);
        } finally {
          fileInput.remove();
        }
      };
      reader.onerror = function (e) {
        updateStatus(`Error: ${e.target.error.message}`);
        console.error("Error: " + e.target.error.message);
        fileInput.remove();
      };
      reader.readAsText(file);
      fileInput.value = "";
    } else {
      updateStatus(`Error: File is too large, he must be < ${maxSizeUpload}ko`);
      fileInput.remove();
    }
  });

  fileInput.addEventListener("cancel", function () {
    fileInput.remove();
  });
}

function startLoadingStatus(message) {
  const spinnerChars = ["⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"];
  let idx = 0;

  if (statusInterval) {
    clearInterval(statusInterval);
    statusInterval = null;
  }

  statusInterval = setInterval(() => {
    idx = (idx + 1) % spinnerChars.length;
    updateStatus(`${message} ${spinnerChars[idx]}`);
  }, 100);
}

function stopLoadingStatus() {
  if (statusInterval) {
    clearInterval(statusInterval);
    statusInterval = null;
  }
}
