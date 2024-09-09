# Windows' windows (_winwin_)

**Quick links:** [documentation](https://yal.cc/docs/gm/winwin/) - [itch](https://yellowafterlife.itch.io/gamemaker-winwin)  
**Versions:** GM2022+ (LTS/Stable/Beta)  
**Platforms:** Windows, Windows (YYC)

This is an extension that lets you create additional windows in GameMaker games on Windows!

This can be used for things like:

- Debug UI (you can even update windows while the game is busy!)
- Pop-out panels and windows (ever make software in GameMaker? Some people do)
- Novel ideas (takes some work to stand out of the crowd these days!)
- "Fourth-wall" tricks (want your game to draw inside and _outside_ the game window?)

And there are always more things to explore!

The extension is straightforward to use - the general functions work much alike to the built-in `window_*` functions, drawing works akin to working with surfaces, and there's a cheat sheet covering what every function does.

And if you prefer to learn from an example, I'm also including a project with small showcases for various things that you might want to do with this.

On the technical side, the extension works directly with WinAPI and D3D11 for best-case performance.
It can be used along with other window-related extensions, and you can usually adjust extensions
to work with a window of your liking even if they don't support multi-window handling out of box.


## Building

See [Compiling extensions](https://yal.cc/gamemaker-windows-extensions-faq/#Compiling-extensions)

## Meta

**Author:** [YellowAfterlife](https://github.com/YellowAfterlife)  
**License:** Custom license (see [LICENSE](LICENSE))