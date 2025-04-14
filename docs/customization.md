# 🌀 Azile Prompt Customization Guide

Azile is a customizable shell prompt that enhances your terminal experience by
displaying useful Git and directory information in a beautiful, minimal style.

You can tweak the appearance of Azile by setting specific environment variables
in your shell's configuration file (e.g., `.bashrc`, `.zshrc`, or `.profile`).

## ✨ Customization Options

Below are the available environment variables you can set to control Azile’s appearance. 
Each variable has a sensible default that will be used if it's not explicitly set.

| Environment Variable              | Description                                      | Default          |
|----------------------------------|--------------------------------------------------|------------------|
| `AZILE_DIR_COLOR`                | Color of the current working directory           | `yellow`         |
| `AZILE_DIR_GIT_SEP`              | Separator string between directory and Git info  | `::`             |
| `AZILE_DIR_GIT_SEP_COLOR`        | Color of the directory/Git separator             | `white`          |
| `AZILE_GIT_COLOR`                | Color of the Git branch name                     | `bright_magenta` |
| `AZILE_GIT_STATUS_SYMBOL`        | Symbol shown when the Git working directory is dirty | `()`       |
| `AZILE_PROMPT_END_SYMBOL`        | Final symbol at the end of the prompt            | ``              |
| `AZILE_PROMPT_END_SYMBOL_COLOR`  | Color of the end prompt symbol                   | `cyan`           |

Azile supports the rendering of NerdFont Icons.

## 🎨 Color Options

Azile supports common color names such as:

- `black`, `red`, `green`, `yellow`, `blue`, `magenta`, `cyan`, `white`
- Bright variants: `bright_red`, `bright_green`, `bright_yellow`, etc.
- Bold variants: `bold_red`, `bold_green`, `bold_yellow`, etc.
- Underline variants: `underline_red`, `underline_green`, `underline_yellow`, etc.

Try different combinations to match your theme or mood!

> Cannot combine bold and underline. Other than that, go wild!

## 🛠️  Example Configurations

<details>
<summary>Bash</summary>
```sh
export AZILE_DIR_COLOR="bright_blue"
export AZILE_DIR_GIT_SEP=" >> "
export AZILE_DIR_GIT_SEP_COLOR="green"
export AZILE_GIT_COLOR="bright_yellow"
export AZILE_GIT_STATUS_SYMBOL="✗"
export AZILE_PROMPT_END_SYMBOL="❯"
export AZILE_PROMPT_END_SYMBOL_COLOR="bright_cyan"
```
</details>

<details>
<summary>Zsh</summary>
```sh
export AZILE_DIR_COLOR="bright_blue"
export AZILE_DIR_GIT_SEP=" >> "
export AZILE_DIR_GIT_SEP_COLOR="green"
export AZILE_GIT_COLOR="bright_yellow"
export AZILE_GIT_STATUS_SYMBOL="✗"
export AZILE_PROMPT_END_SYMBOL="❯"
export AZILE_PROMPT_END_SYMBOL_COLOR="bright_cyan"
```
</details>

## 🚀 Apply Changes

After modifying your shell configuration file:

<details>
<summary>Bash</summary>
```sh
source ~/.bashrc
```
</details>

<details>
<summary>Bash</summary>
```sh
source ~/.zshrc
```
</details>

