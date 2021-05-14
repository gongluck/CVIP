sudo yum install -y gcc gcc-c++ make automake
sudo yum install -y wget
sudo wget https://cmake.org/files/v3.15/cmake-3.15.0.tar.gz
sudo tar -zxvf cmake-3.15.0.tar.gz 
cd cmake-3.15.0/
sudo ./bootstrap
sudo gmake -j4
sudo gmake install
cmake --version
