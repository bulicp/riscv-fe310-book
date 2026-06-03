/* Cosmetic: relabel the theme menu to Catppuccin flavours and hide the
   redundant duplicates. Colour correctness does NOT depend on this file
   (catppuccin.css recolours every theme); this only tidies the menu.
   Robust against newer mdBook that builds the menu after load: it polls
   briefly until the buttons exist. Result:  [Auto]  Latte  Mocha  */
(function () {
    var RENAME = { light: "Latte", coal: "Mocha" };
    var HIDE = ["rust", "navy", "ayu"];

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
        var tries = 0;
        var timer = setInterval(function () {
            if (apply() || ++tries > 40) clearInterval(timer);
        }, 125);
        document.addEventListener("DOMContentLoaded", apply);
    }
})();
