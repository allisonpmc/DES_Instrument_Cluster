# Install Docker

We are using Ubuntu 22.04 LTS version. Based on the Ubuntu version, the installation steps may vary.

- Install Dependencies
```
# Add Docker's official GPG key:
sudo apt-get update
sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc
```

- Creation of a stable repository for Docker
```
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```

- Install related packages for Docker
```
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

- Verify installation with hello world image
```
sudo docker run hello-world
```

- Managing permissions
```
sudo usermod -aG docker ${USER}
su - ${USER}
sudo systemctl enable docker
```

- Install QEMU
```
sudo apt install qemu-system-x86 qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virt-manager
```

- Enable and start  libvirt
```
sudo systemctl enable libvirtd
sudo systemctl start libvirtd
```

- Add Your User to the Libvirt and KVM Groups
```
sudo usermod -aG libvirt $(whoami)
sudo usermod -aG kvm $(whoami)
```

- Verify Installation
```
virsh list --all
```
You should see an empty list.

- Set up QEMU for multi-architecture support
```
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
```

- Create and use a new Buildx builder instance
```
docker buildx create --use --name mybuilder
docker buildx inspect mybuilder --bootstrap
```

- Verify Buildx installation
```
docker buildx ls
```

# Compile Qt 6.6.3 with Docker

Firstly, clone the following repository.
```
git clone https://github.com/PhysicsX/QTonRaspberryPi.git
cd QTonRaspberryPi
```

Run the command to create rasbian(debian) image.
```
docker buildx build --platform linux/arm64 --load -f DockerFileRasp -t raspimage .
```

When it finishes, you will find a file named 'rasp.tar.gz' in the '/build' directory within the image. Let's copy it to the same location where the Dockerfile exists. Just copy it to where you pulled the branch. To copy the file, you need to create a temporary container using the 'create' command. You can delete this temporary container later if you wish
```
docker create --name temp-arm raspimage
docker cp temp-arm:/build/rasp.tar.gz ./rasp.tar.gz
```

Instead of the DockerFile available in the cloned repository, use the DockerFile from here.
-[DockerFile](https://github.com/tweetyboo/DES---Instrument_Cluster/blob/main/Docker/Dockerfile)

To create qtcrossbuild, run the following command
```
docker build -t qtcrossbuild .
```

 After this, you can find HelloQt6 binary which is ready to run on Raspberry pi, in the /build/project directory in the image. So lets copy it. As we did before, you need to create temporary container to copy it.
```
docker create --name tmpbuild qtcrossbuild
docker cp tmpbuild:/build/project/HelloQt6 ./HelloQt6
```

The above step create a Helloworld program for aarch64 architecture. Run the following command.
```
seame:~/QTonRaspberryPi$ file HelloQt6 
HelloQt6: ELF 64-bit LSB executable, ARM aarch64, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, for GNU/Linux 3.7.0, with debug_info, not stripped
```

To test the hello world, you need to copy and send the compiled qt binaries in the image to RPi.
```
docker cp tmpbuild:/build/qt-pi-binaries.tar.gz ./qt-pi-binaries.tar.gz
scp qt-pi-binaries.tar.gz team1@192.168.86.77:/home/team1/
ssh team1@192.168.86.77
team1@raspberrypi:~ sudo mkdir /usr/local/qt6
team1@raspberrypi:~ sudo tar -xvf qt-pi-binaries.tar.gz -C /usr/local/qt6
team1@raspberrypi:~ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/qt6/lib/
seame:scp HelloQt6 team1@192.168.86.77:/home/team1/
```
Make sure to change the pi name and ip address of yours while running scp statement.

To execute the program on RPi, run the following command in RPi.
```
team1@raspberrypi:~ $ ./HelloQt6
Hello world
```

When the initial setup and check is completed, the program files should be placed in the projectt folder in the cloned repository. Then execute the following command and create different containers as required.
```
docker build -t qtcrossbuild .
docker create --name tmpbuild qtcrossbuild
docker cp tmpbuild:/build/project/HelloQt6 ./HelloQt6
scp HelloQt6 team1@192.168.86.77:/home/team1/
```

Instead of HelloQt6, the name has to be changed as per the executable name from your CMakeLists.txt file. Create container with different names or delete the older containers.







