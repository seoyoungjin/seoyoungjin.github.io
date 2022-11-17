set smartindent
set ts=4 sw=4 expandtab

call plug#begin()
    Plug 'preservim/nerdtree'
    Plug 'jremmen/vim-ripgrep'
call plug#end()

let g:rg_command = 'rg --vimgrep -S'
noremap <F7> :<C-U>cprevious<CR>
noremap <F8> :<C-U>cnext<CR>

if has("gui_running")
    colorscheme desert
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
