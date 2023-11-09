```
 ██████╗    ████████╗███████╗███╗   ███╗██████╗ ██╗      █████╗ ████████╗███████╗███████╗
██╔════╝    ╚══██╔══╝██╔════╝████╗ ████║██╔══██╗██║     ██╔══██╗╚══██╔══╝██╔════╝██╔════╝
██║            ██║   █████╗  ██╔████╔██║██████╔╝██║     ███████║   ██║   █████╗  ███████╗
██║            ██║   ██╔══╝  ██║╚██╔╝██║██╔═══╝ ██║     ██╔══██║   ██║   ██╔══╝  ╚════██║
╚██████╗       ██║   ███████╗██║ ╚═╝ ██║██║     ███████╗██║  ██║   ██║   ███████╗███████║
 ╚═════╝       ╚═╝   ╚══════╝╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚══════╝
                                                                                                         
```
# C Templates 🛠️

This repository is dedicated to providing versatile C templates. The main branch serves primarily as a presentation, thus utilizing the specific branches which house various template variants is highly recommended. Each branch is meticulously crafted to accommodate different requirements and preferences.

---

## 💎 Recommendations

- For Linux users, check out this [repository](https://github.com/SECRET-GUEST/actions-for-nautilus) to effortlessly create actions in Nautilus, enabling repository creation and cloning with a single click, given that git is installed on your system.


## 📌 Quick Start

### Linux users

Leveraging the [Actions for Nautilus](https://github.com/SECRET-GUEST/actions-for-nautilus) repository allows for the creation of customized actions, including easy cloning of this repository with just one click, provided Git is installed on your system.

### Windows users

You can just clone the `nautilus` branch:

```shell
git clone https://github.com/SECRET-GUEST/cTemplate.git --branch nautilus
```

you can also use a batch script to make a **one-click installer** for anytime you need to clone :

The batch script below assists in cloning a specific git repository. Before running the script, ensure that Git is installed on your system. You can get it from [Git's official site](https://git-scm.com/). 

Here is a brief explanation of the script:

1. It first checks whether Git is installed on your system.
2. If Git is installed, it proceeds to clone the 'main' branch of the specified repository (in this case, the pyTemplate repository).
3. If an error occurs at any point (like Git not being installed or the repository failing to clone), it displays an appropriate error message.

```batch
@echo off
:: Check if git is installed
where git >nul 2>nul
if %errorlevel% neq 0 (
    echo Git is not installed. Please install it from https://git-scm.com/ to proceed.
    exit /b 1
)

:: Clone the repository
git clone https://github.com/SECRET-GUEST/cTemplate.git -b nautilus

if %errorlevel% neq 0 (
    echo An error occurred while cloning the repository.
    exit /b 1
)

echo The repository was cloned successfully.
exit /b 0
```

Save this script as a `.bat` file and execute it to clone the repository. Once the repository is cloned successfully, a confirmation message will be displayed.



## ❓ Support & Questions

For any queries or support needs, please feel free to open an issue or start a discussion. 

## 📜 License

This project is licensed under the [MIT License](LICENSE).
