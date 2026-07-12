document.addEventListener("DOMContentLoaded", function () {
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
});

export function download() {
  const payload = document.getElementById("payload").value;
  const status = document.getElementById("status");

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
      status.textContent = "File downloaded correctly";
    }
  } else {
    status.textContent = "Cannot download an empty file";
  }
}

export async function run() {
  const payload = document.getElementById("payload").value;
  const status = document.getElementById("status");

  status.textContent = "Payload sending ...";

  try {
    const response = await fetch("/run", {
      method: "POST",
      headers: {
        "Content-Type": "application/x-www-form-urlencoded",
      },
      body: "script=" + encodeURIComponent(payload),
    });
    const result = await response.text();
    status.textContent = `${result}`;
  } catch (error) {
    status.textContent = `Error: ${error.message}`;
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
    const status = document.getElementById("status");

    const file = e.target.files[0];

    if (!file) {
      console.error("Select a file please");
    }

    status.textContent = `File: ${file.name} (${file.size} bytes) has been uploaded`;

    const reader = new FileReader();
    reader.onload = function (event) {
      try {
        const content = event.target.result;
        payload.value = content;
        fileInput.remove();
      } catch (error) {
        status.textContent = `Error: ${error.message}`;
        console.error("Error: " + error.message);
      }
      reader.error = function () {
        status.textContent = `Error: ${error.message}`;
        console.error("Error: " + error.message);
      };
    };
    reader.readAsText(file);
    fileInput.value = "";
    fileInput.remove();
  });

  fileInput.addEventListener("cancel", function () {
    fileInput.remove();
  });
}
