/* Cosmetic only (mdBook 0.5.x): rename theme menu entries to Catppuccin
   flavours and hide the redundant duplicates. Colour correctness comes
   entirely from theme/css/variables.css + the highlight overrides, so even
   if this script does nothing the colours are already correct.
   Menu after this runs:  Auto  Latte  Mocha  */
(function () {
    var RENAME = { "mdbook-theme-light": "Latte", "mdbook-theme-coal": "Mocha" };
    var HIDE = ["mdbook-theme-rust", "mdbook-theme-navy", "mdbook-theme-ayu"];

    function apply() {
        var found = false;
        Object.keys(RENAME).forEach(function (id) {
            var b = document.getElementById(id);
            if (b) { b.textContent = RENAME[id]; found = true; }
        });
        HIDE.forEach(function (id) {
            var b = document.getElementById(id);
            if (b) {
                var li = b.closest ? b.closest("li") : b.parentElement;
                if (li) li.style.display = "none";
            }
        });
        return found;
    }
    if (!apply()) {
        var n = 0, t = setInterval(function () {
            if (apply() || ++n > 40) clearInterval(t);
        }, 125);
        document.addEventListener("DOMContentLoaded", apply);
    }
})();
