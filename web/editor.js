import { run, download } from "./main.js"

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

function handleShortCut(event, key, fn) {
  if (event.ctrlKey && event.key == key) {
    event.preventDefault();
    fn();
  }
}
