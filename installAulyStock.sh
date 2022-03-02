sudo apt-get install libmysqlclient-dev
sudo apt install mysql-server
sudo apt-get install build-essential
sudo apt-get install libcurl4-gnutls-dev libcurl4-nss-dev libcurl4-openssl-dev
sudo apt-get install libgtk-3-dev
cmake -S ./ -B ./test/
make -C test/
