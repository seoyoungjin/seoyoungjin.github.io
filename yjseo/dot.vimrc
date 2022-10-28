set smartindent
set ts=4 sw=4 expandtab

colorscheme molokai
if has("gui_running")
    se go-=T
    se go+=m
    "behave mswin
    if has("win32") || has("win64")
        set guifont=DejaVu_Sans_Mono:h12:cANSI
        "map <F6> <ESC>:se guifont=DotumChe:h12:cHANGEUL<CR>:se nocursorline<CR>:se nocursorcolumn<CR>
    elseif has("gui_gtk")
        set guifont=DejaVu\ Sans\ Mono\ 12
    elseif has("gui_macvim")
        set guifont=DejaVu\ Sans\ Mono:h14.00
    endif
else
    colorscheme default
endif

call plug#begin('~/.vim/plugged')
  Plug 'preservim/nerdtree'
  Plug 'timonv/vim-cargo'
  Plug 'rust-lang/rust.vim'
call plug#end()
