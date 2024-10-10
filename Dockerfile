FROM epitechcontent/epitest-docker
COPY network/conanfile.txt .

RUN ["apt-get", "update", "-y"]
RUN ["apt-get", "upgrade", "-y"]
RUN ["apt-get", "install", "pipx", "-y"]

RUN ["pipx", "install", "conan"]
RUN ["mv", "/root/.local/bin/conan", "/usr/bin/"]

RUN ["apt-get", "install", "libx11-dev", "libx11-xcb-dev", "libfontenc-dev", "libice-dev", "libsm-dev", "libxau-dev", "libxaw7-dev", "libxcomposite-dev", "libxdamage-dev", "libxkbfile-dev", "libxmu-dev", "libxmuu-dev", "libxres-dev", "libxtst-dev", "libxcb-glx0-dev", "libxcb-render0-dev", "libxcb-render-util0-dev", "libxcb-xkb-dev", "libxcb-icccm4-dev", "libxcb-keysyms1-dev", "libxcb-randr0-dev", "libxcb-shape0-dev", "libxcb-sync-dev", "libxcb-xfixes0-dev", "libxcb-xinerama0-dev", "libxcb-dri3-dev", "libxcb-cursor-dev", "libxcb-dri2-0-dev", "libxcb-dri3-dev", "libxcb-present-dev", "libxcb-composite0-dev", "libxcb-ewmh-dev", "libxcb-res0-dev", "-y"]

RUN ["conan", "profile", "detect"]
RUN ["conan", "install", ".", "--output-folder=build",  "--build=missing"]
