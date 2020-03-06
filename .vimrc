syntax on
colo pablo


" Flash screen instead of beep sound
" set visualbell

" Change how vim represents characters on the screen
set encoding=utf-8

" Set the encoding of files written
set fileencoding=utf-8
set tabstop=4

"autocmd Filetype python setlocal expandtab tabstop=4 shiftwidth=4 softtabstop=4
"autocmd Filetype go setlocal tabstop=4 shiftwidth=4 softtabstop=4
" ts - show existing tab with 4 spaces width
" sw - when indenting with '>', use 4 spaces width
" sts - control <tab> and <bs> keys to match tabstop

" Control all other files
set shiftwidth=4
set nocompatible
set showcmd
set undofile " Maintain undo history between sessions
set undodir=~/.vim/undodir

" Hardcore mode, disable arrow keys.
" noremap <Up> <NOP>
" noremap <Down> <NOP>
" noremap <Left> <NOP>
" noremap <Right> <NOP>

filetype plugin indent on
runtime macros/matchit.vim

"setlocal foldmethod=syntax
"set foldenable
"set foldmethod=indent

set spell
set spelllang=en,ru

" Allow backspace to delete indentation and inserted text
" i.e. how it works in most programs
set backspace=indent,eol,start
" indent  allow backspacing over autoindent
" eol     allow backspacing over line breaks (join lines)
" start   allow backspacing over the start of insert; CTRL-W and CTRL-U
"        stop once at the start of insert.


" go-vim plugin specific commands
" Also run `goimports` on your current file on every save
" Might be be slow on large codebases, if so, just comment it out
let g:go_fmt_command = "goimports"

" Status line types/signatures.
let g:go_auto_type_info = 1
let g:go_def_mode='gopls'
let g:go_info_mode='gopls'

"au BufRead,BufNewFile *.html set filetype=gohtmltmpl
"au filetype go inoremap <buffer> . .<C-x><C-o>
"
" If you want to disable gofmt on save
" let g:go_fmt_autosave = 0

" Перенос строк
set breakindent
set formatoptions=l
set lbr

autocmd BufWinLeave *.* mkview
autocmd BufWinEnter *.* silent loadview

let mapleader = ","
noremap <leader>/ :Commentary<CR>

nnoremap x "_x
nnoremap d "_d
nnoremap D "_D
vnoremap d "_d

nnoremap <leader>d ""d
nnoremap <leader>D ""D
vnoremap <leader>d ""d

" NERDTree plugin specific commands
:nnoremap <C-g> :NERDTreeToggle<CR>
" let g:NERDTreeMinimalUI = 1
"autocmd vimenter * NERDTree

fu! MyTabLabel(n)
let buflist = tabpagebuflist(a:n)
let winnr = tabpagewinnr(a:n)
let string = fnamemodify(bufname(buflist[winnr - 1]), ':t')
return empty(string) ? '[unnamed]' : string
endfu

fu! MyTabLine()
let s = ''
for i in range(tabpagenr('$'))
" select the highlighting
    if i + 1 == tabpagenr()
    let s .= '%#TabLineSel#'
    else
    let s .= '%#TabLine#'
    endif

    " set the tab page number (for mouse clicks)
    "let s .= '%' . (i + 1) . 'T'
    " display tabnumber (for use with <count>gt, etc)
    let s .= ' '. (i+1) . ' ' 

    " the label is made by MyTabLabel()
    let s .= ' %{MyTabLabel(' . (i + 1) . ')} '

    if i+1 < tabpagenr('$')
        let s .= ' |'
    endif
endfor
return s
endfu
set tabline=%!MyTabLine()

" use `:OR` for organize import of current buffer
autocmd BufWritePre *.go :OR
autocmd BufWritePre *.go :Format

let g:go_code_completion_enabled = 0
let g:go_gopls_complete_unimported = 1
let g:go_gopls_deep_completion = 0
"let g:go_doc_popup_window = 1
let g:go_updatetime = 200
let g:go_highlight_types = 1
"let g:go_highlight_fields = 1
let g:go_highlight_functions = 1
let g:go_highlight_extra_types = 1
let g:go_highlight_function_parameters = 1
let g:go_highlight_variable_declarations = 1
let g:go_highlight_variable_assignments = 1
"let g:go_auto_sameids = 1
"let g:go_highlight_string_spellcheck = 1

"let g:ale_sign_warning = '⚠' " Enable integration with airline.
" air-line plugin specific commands
let g:airline_powerline_fonts = 1

if !exists('g:airline_symbols')
    let g:airline_symbols = {}
endif

let g:airline_extensions = ['coc']
let g:airline#extensions#coc#enabled = 1
let g:airline#extensions#coc#error_symbol = 'E:'
let g:airline#extensions#coc#warning_symbol = 'W:'
let g:airline#extensions#coc#stl_format_err = '%E{[%e(#%fe)]}'
let g:airline#extensions#coc#stl_format_warn = '%W{[%w(#%fw)]}'
" unicode symbols
"let g:airline_left_sep = '»'
"let g:airline_left_sep = '▶'
"let g:airline_right_sep = '«'
"let g:airline_right_sep = '◀'
"let g:airline_symbols.linenr = '␊'
"let g:airline_symbols.linenr = '␤'
"let g:airline_symbols.linenr = '¶'
"let g:airline_symbols.branch = '⎇'
"let g:airline_symbols.paste = 'ρ'
"let g:airline_symbols.paste = 'Þ'
"let g:airline_symbols.paste = '∥'
"let g:airline_symbols.whitespace = 'Ξ'

" airline symbols
"let g:airline_left_sep = ''
"let g:airline_left_alt_sep = ''
"let g:airline_right_sep = ''
"let g:airline_right_alt_sep = ''
"let g:airline_symbols.branch = ''
"let g:airline_symbols.readonly = ''
"let g:airline_symbols.linenr = ''

" vim-devicons
" let g:webdevicons_enable = 1
" let g:webdevicons_enable_nerdtree = 1
" let g:webdevicons_enable_unite = 1
" let g:webdevicons_enable_vimfiler = 1
" let g:webdevicons_enable_airline_tabline = 1
" let g:webdevicons_enable_airline_statusline = 1
" let g:webdevicons_enable_ctrlp = 1
" let g:webdevicons_enable_flagship_statusline = 1
" let g:WebDevIconsUnicodeDecorateFileNodes = 1
" let g:WebDevIconsUnicodeGlyphDoubleWidth = 1
" let g:webdevicons_conceal_nerdtree_brackets = 1
" let g:WebDevIconsNerdTreeAfterGlyphPadding = '  '
" let g:WebDevIconsNerdTreeGitPluginForceVAlign = 1
" let g:webdevicons_enable_denite = 1
" let g:WebDevIconsUnicodeDecorateFolderNodes = 1
" let g:DevIconsEnableFoldersOpenClose = 1
" let g:DevIconsEnableFolderPatternMatching = 1
" let g:DevIconsEnableFolderExtensionPatternMatching = 1
" let WebDevIconsUnicodeDecorateFolderNodesExactMatches = 1
let g:WebDevIconsUnicodeGlyphDoubleWidth=0
let g:WebDevIconsNerdTreeAfterGlyphPadding = ' '
let g:WebDevIconsUnicodeDecorateFolderNodes = v:true
let g:WebDevIconsNerdTreeGitPluginForceVAlign = 1


call plug#begin()
"Plug 'Valloric/YouCompleteMe'
"Plug 'powerline/powerline-fonts'
Plug 'ryanoasis/vim-devicons'
Plug 'tpope/vim-commentary'
Plug 'vim-airline/vim-airline'
"Plug 'fatih/molokai'
Plug 'morhetz/gruvbox'
Plug 'chrisbra/changesPlugin'
Plug 'majutsushi/tagbar'
"Plug 'fatih/vim-go'
"Plug 'neoclide/coc-go', {'do': 'yarn install --frozen-lockfile'}
Plug 'neoclide/coc.nvim', {'do': 'yarn install --frozen-lockfile'}
"Plug 'neoclide/coc-tsserver', {'do': 'yarn install --frozen-lockfile'}
"Plug 'neoclide/coc-css', {'do': 'yarn install --frozen-lockfile'}
"Plug 'neoclide/coc-highlight', {'do': 'yarn install --frozen-lockfile'}
"Plug 'neoclide/coc-json', {'do': 'yarn install --frozen-lockfile'}
"Plug 'neoclide/coc-html', {'do': 'yarn install --frozen-lockfile'}
" On-demand loading
Plug 'scrooloose/nerdtree', { 'on': 'NERDTreeToggle' }
Plug 'aserebryakov/vim-todo-lists'
Plug 'lyokha/vim-xkbswitch'
call plug#end()

"au BufNewFile,BufRead *.xml,*.htm,*.html so XMLFolding
set number
set cursorline

colorscheme gruvbox
let g:gruvbox_italic = 1
let g:gruvbox_contrast_dark = "hard"
"hi NonText ctermbg=none
hi Normal guibg=NONE ctermbg=NONE
highlight Comment cterm=italic
" highlight CursorLine ctermbg=130
"hi Cursor guifg=white guibg=black
"hi iCursor guifg=white guibg=steelblue
"set guicursor=n-v-c:block-Cursor
"set guicursor+=i:ver100-iCursor
"set guicursor+=n-v-c:blinkon0
"set guicursor+=i:blinkwait10

"let g:VimTodoListsDatesEnabled = 1
let g:VimTodoListsMoveItems = 0

"let g:XkbSwitchLib = "/usr/lib/libxkbswitch.so"
"echo libcall(g:XkbSwitchLib, 'Xkb_Switch_getXkbLayout', '')
"call libcall(g:XkbSwitchLib, 'Xkb_Switch_setXkbLayout', 'us')

" if $DISPLAY == "" 
" 	let g:XkbSwitchEnabled = 1
" else
" 	let g:XkbSwitchEnabled = 1
" 	let g:XkbSwitchLib = '/usr/lib/libxkbswitch.so'
" 	let g:XkbSwitchIMappings = ['en']
" endif 
"let g:XkbSwitchIMappings = ['ru']
"let g:airline#extensions#xkblayout#enabled = 1
"let g:airline#extensions#keymap#enabled = 1

"set keymap=russian-jcukenwin
"set iminsert=0
"set imsearch=0
"set langmap=ФИСВУАПРШОЛДЬТЩЗЙКЫЕГМЦЧНЯ;ABCDEFGHIJKLMNOPQRSTUVWXYZ,фисвуапршолдьтщзйкыегмцчня;abcdefghijklmnopqrstuvwxyz

let rumap = 'йцукенгшщзхъфывапролджэёячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ'
let enmap = 'qwertyuiop[]asdfghjkl;''\zxcvbnm,.QWERTYUIOP{}ASDFGHJKL:"ZXCVBNM<>' 
let mapLen = strchars(rumap)
let i = 0
while i < mapLen
    let ruChar = matchstr(rumap, ".", byteidx(rumap, i))
    let enChar = enmap[i]
    "echo 'map '.ruChar.' '.enChar
    execute 'map '.ruChar.' '.enChar
    execute 'cmap '.ruChar.' '.enChar
    let i += 1
endwhile

"map Ё \|
"cmap Ё \|

inoremap <C-h> <Left>
inoremap <C-j> <Down>
inoremap <C-k> <Up>
inoremap <C-l> <Right>
cnoremap <C-h> <Left>
cnoremap <C-j> <Down>
cnoremap <C-k> <Up>
cnoremap <C-l> <Right>

nnoremap <Leader>j :m .+1<CR>==
nnoremap <Leader>k :m .-2<CR>==
inoremap <Leader>j <Esc>:m .+1<CR>==gi
inoremap <Leader>k <Esc>:m .-2<CR>==gi
vnoremap <Leader>j :m '>+1<CR>gv=gv
vnoremap <Leader>k :m '<-2<CR>gv=gv

"let g:rehash256 = 1
"let g:molokai_original = 1
"colorscheme molokai

set lazyredraw

" let NERDTreeMapOpenInTab='<ENTER>'

"let g:tagbar_ctags_bin = '/home/magy/.vim/ctags'
let g:tagbar_type_go = {
	\ 'ctagstype' : 'go',
	\ 'kinds'     : [
		\ 'p:package',
		\ 'i:imports:1',
		\ 'c:constants',
		\ 'v:variables',
		\ 't:types',
		\ 'n:interfaces',
		\ 'w:fields',
		\ 'e:embedded',
		\ 'm:methods',
		\ 'r:constructor',
		\ 'f:functions'
	\ ],
	\ 'sro' : '.',
	\ 'kind2scope' : {
		\ 't' : 'ctype',
		\ 'n' : 'ntype'
	\ },
	\ 'scope2kind' : {
		\ 'ctype' : 't',
		\ 'ntype' : 'n'
	\ },
	\ 'ctagsbin'  : 'gotags',
	\ 'ctagsargs' : '-sort -silent'
\ }

let g:go_def_mapping_enabled = 0

" if hidden is not set, TextEdit might fail.
set hidden

" Some servers have issues with backup files, see #649
set nobackup
set nowritebackup

" Better display for messages
set cmdheight=2

" You will have bad experience for diagnostic messages when it's default 4000.
set updatetime=100

" don't give |ins-completion-menu| messages.
set shortmess+=c

" always show signcolumns
set signcolumn=yes

"let g:coc_global_extensions = [
"  \ 'coc-go',
"  \ 'coc-html',
"  \ 'coc-tsserver',
"  \ 'coc-css', 
"  \ 'coc-json', 
"  \ ]

" Use tab for trigger completion with characters ahead and navigate.
" Use command ':verbose imap <tab>' to make sure tab is not mapped by other plugin.
inoremap <silent><expr> <TAB>
      \ pumvisible() ? "\<C-n>" :
      \ <SID>check_back_space() ? "\<TAB>" :
      \ coc#refresh()
inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"

function! s:check_back_space() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction

" Use <c-space> to trigger completion.
inoremap <silent><expr> <c-space> coc#refresh()

" Use <cr> to confirm completion, `<C-g>u` means break undo chain at current position.
" Coc only does snippet and additional edit on confirm.
inoremap <expr> <cr> pumvisible() ? "\<C-y>" : "\<C-g>u\<CR>"
" Or use `complete_info` if your vim support it, like:
" inoremap <expr> <cr> complete_info()["selected"] != "-1" ? "\<C-y>" : "\<C-g>u\<CR>"

" Use `[g` and `]g` to navigate diagnostics
nmap <silent> [g <Plug>(coc-diagnostic-prev)
nmap <silent> ]g <Plug>(coc-diagnostic-next)

" Remap keys for gotos
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)

"set completeopt+=preview
"set statusline^=preview


" Use K to show documentation in preview window
nnoremap <silent> K :call <SID>show_documentation()<CR>
" nnoremap <silent> L :call StatusDiagnostic()<CR>

autocmd User CocJumpPlaceholder call CocActionAsync('showSignatureHelp')

function! s:show_documentation()
  if (index(['vim','help'], &filetype) >= 0)
    execute 'h '.expand('<cword>')
  else
    call CocAction('doHover')
  endif
endfunction

function! StatusDiagnostic() abort
            let info = get(b:, 'coc_diagnostic_info', {})
            if empty(info) | return '' | endif
            let msgs = []
            if get(info, 'error', 0)
              call add(msgs, 'E' . info['error'])
            endif
            if get(info, 'warning', 0)
              call add(msgs, 'W' . info['warning'])
            endif
            return join(msgs, ' ') . ' ' . get(g:, 'coc_status', '')
endfunction

echo StatusDiagnostic()
"set statusline=
"set statusline+=%{coc#doHover()}
"set statusline^=%{coc#status()}%{get(b:,'coc_current_function','')}

" Highlight symbol under cursor on CursorHold
autocmd CursorHold * silent call CocActionAsync('highlight')

" Remap for rename current word
nmap <leader>rn <Plug>(coc-rename)

" Remap for format selected region
xmap <leader>f  <Plug>(coc-format-selected)
nmap <leader>f  <Plug>(coc-format-selected)

augroup mygroup
  autocmd!
  " Setup formatexpr specified filetype(s).
  autocmd FileType typescript,json setl formatexpr=CocAction('formatSelected')
  " Update signature help on jump placeholder
  autocmd User CocJumpPlaceholder call CocActionAsync('showSignatureHelp')
augroup end

" Remap for do codeAction of selected region, ex: `<leader>aap` for current paragraph
xmap <leader>a  <Plug>(coc-codeaction-selected)
nmap <leader>a  <Plug>(coc-codeaction-selected)

" Remap for do codeAction of current line
nmap <leader>ac  <Plug>(coc-codeaction)
" Fix autofix problem of current line
nmap <leader>qf  <Plug>(coc-fix-current)

" Create mappings for function text object, requires document symbols feature of languageserver.
xmap if <Plug>(coc-funcobj-i)
xmap af <Plug>(coc-funcobj-a)
omap if <Plug>(coc-funcobj-i)
omap af <Plug>(coc-funcobj-a)

" Use <C-d> for select selections ranges, needs server support, like: coc-tsserver, coc-python
nmap <silent> <C-d> <Plug>(coc-range-select)
xmap <silent> <C-d> <Plug>(coc-range-select)

" Use `:Format` to format current buffer
command! -nargs=0 Format :call CocAction('format')

" Use `:Fold` to fold current buffer
command! -nargs=? Fold :call     CocAction('fold', <f-args>)

" use `:OR` for organize import of current buffer
command! -nargs=0 OR   :call     CocAction('runCommand', 'editor.action.organizeImport')

" Add status line support, for integration with other plugin, checkout `:h coc-status`
set statusline^=%{coc#status()}%{get(b:,'coc_current_function','')}

highlight link CocErrorSign GruvboxBlue


" Using CocList
" Show all diagnostics
nnoremap <silent> <space>a  :<C-u>CocList diagnostics<cr>
" Manage extensions
nnoremap <silent> <space>e  :<C-u>CocList extensions<cr>
" Show commands
nnoremap <silent> <space>c  :<C-u>CocList commands<cr>
" Find symbol of current document
nnoremap <silent> <space>o  :<C-u>CocList outline<cr>
" Search workspace symbols
nnoremap <silent> <space>s  :<C-u>CocList -I symbols<cr>
" Do default action for next item.
nnoremap <silent> <space>j  :<C-u>CocNext<CR>
" Do default action for previous item.
nnoremap <silent> <space>k  :<C-u>CocPrev<CR>
" Resume latest coc list
nnoremap <silent> <space>p  :<C-u>CocListResume<CR>
