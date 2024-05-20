arch_install() {
  sudo pacman -Su assimp glfw glew cmake
}

manjaro_install() {
  pamac install cmake assimp glfw glew
}

apt_install() {
  apt update
  apt install cmake libassimp-dev libglfw3 libglfw3-dev libglew-dev
}


source /etc/os-release
case $ID in
  arch)
    arch_install
    ;;
  manjaro)
    manjaro_install
    ;;
  debian|ubuntu|mint)
    apt_install
    ;;

  *)
    echo -n "unsupported linux distro"
    ;;
esac
