FROM epitechcontent/epitest-docker
COPY . .
RUN ["apt-get", "update", "-y"]
RUN ["apt-get", "upgrade", "-y"]
RUN ["apt-get", "install", "pipx", "-y"]
RUN ["pipx", "install", "conan"]
