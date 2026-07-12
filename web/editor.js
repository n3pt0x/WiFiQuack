import { run, download } from "./main.js";

document.addEventListener("keydown", function (e) {
  handleShortCut(e, "Enter", function () {
    run();
  });
});

document.addEventListener("keydown", function (e) {
  handleShortCut(e, "s", function () {
    download();
  });
});

document.getElementById("copyBtn").addEventListener("click", function (e) {
  clipboard();
});

document.getElementById("cleanBtn").addEventListener("click", () => {
  document.getElementById("payload").value = "";
});

function handleShortCut(event, key, fn) {
  if (event.ctrlKey && event.key == key) {
    event.preventDefault();
    fn();
  }
}

function clipboard() {
  const payload = document.getElementById("payload");
  const status = document.getElementById("status");

  payload.select();
  document.execCommand("copy");
  payload.setSelectionRange(0, 0);

  status.textContent = "📋 Copied !";

  setTimeout(() => {
    status.textContent = "";
  }, 2000);
}
