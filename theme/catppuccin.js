/* Relabel the mdBook theme menu so it reads as Catppuccin flavours,
   and hide the themes we don't style. Purely cosmetic. */
(function () {
    function apply() {
        var list = document.getElementById("theme-list");
        if (!list) return;

        var rename = { light: "Latte", coal: "Mocha" };
        Object.keys(rename).forEach(function (id) {
            var btn = document.getElementById(id);
            if (btn) btn.textContent = rename[id];
        });

        ["rust", "navy", "ayu"].forEach(function (id) {
            var btn = document.getElementById(id);
            if (btn) {
                var li = btn.closest ? btn.closest("li") : btn.parentElement;
                if (li) li.style.display = "none";
            }
        });
    }

    if (document.readyState !== "loading") apply();
    else document.addEventListener("DOMContentLoaded", apply);
})();
