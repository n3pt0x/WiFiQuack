import { run, download } from "./main.js";

export const STORAGE_SCRIPT_KEY = "ducky_script";

const shortcuts = {
  Enter: run,
  s: download,
  S: download,
};

/* eventListener */

// localStorage

let saveTimeout = null;

document.getElementById("payload").addEventListener("input", () => {
  if (saveItem) {
    clearTimeout(saveTimeout);
  }

  saveTimeout = setTimeout(() => {
    const payload = document.getElementById("payload").value;
    saveItem(STORAGE_SCRIPT_KEY, payload);
  }, 2000);
});

document.addEventListener("DOMContentLoaded", () => {
  const saved = getItem(STORAGE_SCRIPT_KEY);
  if (saved !== null) {
    document.getElementById("payload").value = saved;
  }
});

// Shortcut

document.addEventListener("keydown", function (e) {
  if (!e.ctrlKey) return;

  const fn = shortcuts[e.key];
  if (fn) {
    e.preventDefault();
    fn();
  }
});

// Button

document.getElementById("copyBtn").addEventListener("click", function (e) {
  clipboard();
});

document.getElementById("cleanBtn").addEventListener("click", () => {
  document.getElementById("payload").value = "";
  removeItem(STORAGE_SCRIPT_KEY);
  updateStatus("🧹 Cleared !", 2000);
});

/* function */

function clipboard() {
  const payload = document.getElementById("payload");
  const status = document.getElementById("status");

  payload.select();
  document.execCommand("copy");
  payload.setSelectionRange(0, 0);

  updateStatus("📋 Copied !", 2000);
}

export function updateStatus(message, delay = 0) {
  const status = document.getElementById("status");
  status.textContent = message;

  if (delay > 0) {
    setTimeout(() => {
      status.textContent = "";
    }, delay);
  }
}

export function getItem(key) {
  try {
    return localStorage.getItem(key);
  } catch (error) {
    console.error("Error saving to localStorage: ", error);
    return false;
  }
}

export function saveItem(key, payload) {
  if (payload != "") {
    try {
      localStorage.setItem(key, payload);
      return true;
    } catch (error) {
      console.error("Error saving to localStorage: ", error);
      return false;
    }
  }
  return false;
}

export function removeItem(key) {
  try {
    localStorage.removeItem(key);
    return true;
  } catch (error) {
    console.error("Error saving to localStorage: ", error);
    return false;
  }
}
