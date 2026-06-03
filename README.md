# Bare-metal RISC-V on the SiFive FE310

A short, hands-on book on **memory-mapped I/O** and **interrupts** on the SiFive
**FE310-G002** RISC-V microcontroller, with complete, buildable C and assembly
source code. It is written with [mdBook](https://rust-lang.github.io/mdBook/) and
publishes to GitHub Pages.

> 📖 **Read it online:** https://bulicp.github.io/riscv-fe310-book/

## What's inside

**Part I — Memory-mapped I/O: GPIO and the UART**
What memory-mapped I/O means; the FE310's GPIO interface driven from both
assembly and C; then the UART serial protocol, the FE310's UART registers,
driving it from C by mirroring the registers with a `struct`, and routing it onto
pins via the GPIO IO-function multiplexer.

**Part II — RISC-V interrupts and exceptions**
Privileged modes, the five machine-mode CSRs (`mstatus`, `mie`, `mip`, `mcause`,
`mtvec`), the FE310's CLINT and PLIC, interrupt entry/exit, building a vector
table and handlers, and a working **round-robin task scheduler** driven by the
machine timer interrupt.

## Repository layout

```
.
├── book.toml                 mdBook configuration
├── src/                      the book's Markdown source
│   ├── SUMMARY.md            table of contents
│   ├── introduction.md
│   ├── mmio/                 Part I
│   ├── interrupts/           Part II
│   └── images/               figures
├── code/                     companion source code (see code/README.md)
│   ├── drivers/
│   ├── interrupts/
│   ├── scheduler/
│   └── snippets/
├── theme/custom.css          small styling tweaks
└── .github/workflows/        GitHub Pages deployment
```

The [Source code map](src/appendix-source.md) links every source file to the
section that discusses it.

## Theme

The book ships with a self-contained [Catppuccin](https://catppuccin.com/) theme
(no preprocessor needed): the light flavour is **Latte**, the dark flavour is
**Mocha**, selectable from the paintbrush menu in the top bar. The mapping lives
in `theme/catppuccin.css` (palette + syntax colours) and `theme/catppuccin.js`
(menu labels); tweak the hex values there to taste. To go back to the stock
mdBook themes, remove those two files from the `additional-css`/`additional-js`
lists in `book.toml`.

## Building locally

```bash
# install mdBook (needs Rust's cargo, or grab a release binary from
# https://github.com/rust-lang/mdBook/releases)
cargo install mdbook

# live preview at http://localhost:3000
mdbook serve --open

# or just build the static site into ./book
mdbook build
```

## Publishing to GitHub Pages

1. Push this repository to GitHub.
2. In **Settings → Pages**, set **Source** to **GitHub Actions**.
3. Push to `main`. The workflow in
   [`.github/workflows/deploy.yml`](.github/workflows/deploy.yml) builds the book
   and deploys it automatically.

Then replace every `bulicp` placeholder (in this file, in `book.toml`, and in
the cross-links inside `src/`) with your GitHub username.

## Attribution & licence

The text is adapted from the *Memory-mapped Input/Output* and *Interrupts and
interrupt handling* chapters of Patricio Bulić's *Understanding Computer
Organization*; the source code is the author's FE310 driver and context-switch
code. Add a `LICENSE` file with the terms you intend to release under before
publishing.
