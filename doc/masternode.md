
# Ksoc Masternode Setup Guide

A short guide on how to create a Masternode for Ksoc Coin.

## Local Wallet Setup

These first steps are for the wallet that will be holding your Ksoc Collateral. (10.000 KSOCs)



If you have atleast 10.000 KSOCs coins, we can go to the MASTERNODES tab and from there select "Create Masternode Controller"



The masternode controller will walk you through the steps to setup everything required on your local wallet.

![image](https://user-images.githubusercontent.com/38903436/204892391-5865bf3f-9f93-43de-92c3-613b9e78461e.png)


This screen we will have to add our name for the masternode, which if you see my screenshots mine are "Masternode1" but you can call it anything you like, many will commonly do "mn1" "mn2" so on and so forth.

![image](https://user-images.githubusercontent.com/38903436/204892614-124b6519-1b08-4c52-9b55-f3c6ebce26c0.png)


This next screen is requesting the IP address for the VPS you have purchased in order to setup your masternode. 

![image](https://user-images.githubusercontent.com/38903436/204893013-bb4d6782-a162-44a4-9af8-018039684ea2.png)

Some places that you can get one are:<br><br/>
https://hostmaria.com <br/>
https://contabo.com/en/vps/ <br/>
https://www.vultr.com/ <br/>

After clicking next, it will automatically send the coins to a new address and lock them to setup a masternode. The sent transaction requires atleast 15 confirmations before you can try starting the masternode, but that should be more than enough time to start configuring the VPS and it should all be ready by the time it is complete.

Upon completion we need to save some data we can get from our local wallet. If you are still in the masternode tab you need to click on the 3 dots next to your newly created masternode name. After clicking on it there is a tab called Info and it will have a copy button.



Now, we need to click on the copy button by "Export data to run the masternode on a remote server", this will copy to your clipboard information like the following:

```
masternode=1
externalip=51.161.166.129 (example IP)
masternodeaddr=51.161.166.129:20555 
masternodeprivkey=88wsw6D2KTQzSxEWD6WFYGtb9CcYqecGKA6UjTqkda313kr4VzD
```

We will save this information in a notepad somewhere for later after we start configuring our VPS.

## VPS Setup

This portion of the guide makes several assumptions:
- Already purchased a VPS
- Understanding of SSH
- Understanding of basic linux commands

For your VPS it is recommended to use minimally Ubuntu 18.04 for the Operating System.

Upon logging into your VPS with SSH, you will need to do several things

- Download the linux wallet
- Setup the ksoc.conf file
- Start the masternode
- Check the status

First things first lets update the system with the following commands
```
sudo apt-get update
sudo apt-get upgrade
```
This will take a few moments and upon completion we can simply run this command to download the wallet and extract it.
```
wget -c https://github.com/ksocsports/ksoc/releases/download/4.3.3/ksoc-linux.static.tar.gz && tar -xvf ksoc-linux.static.tar.gz 
```
Next, we will configure our masternode we will start by making the default coin directory and configuration files below.
```
mkdir /root/.ksoc
nano /root/.ksoc/ksoc.conf
```
Here we will need to use that saved information from our local wallet and copy and paste it into the conf file.
You will also have to add the following
```
externalip=IP_HERE
rpcbind=127.0.0.1
bind=IP_HERE
rpcuser=supersecureusername
rpcpassword=supersecurepassword
rpcallowip=127.0.0.1
server=1
daemon=1
listen=1
maxconnections=256
masternode=1
externalip=51.161.166.129
masternodeaddr=51.161.166.129:20555
masternodeprivkey=88wsw6D2KTQzSxEWD6WFYGtb9CcYqecGKA6UjTqkda313kr4VzD
```
Theres a few modifications to the places, with IP_HERE we would replace with the IP address of the VPS that we are logged into. The RPC username/passwords need to be set for checking and talking to your masternode. RPC Port can be adjusted by Port can not.

Afterwards save and close the conf file. Now we should update core with last block, for this:
```
cd /root/.ksoc/
mkdir blocks && cd blocks
wget -c https://github.com/ksocsports/ksoc/releases/download/4.3.2/bootstrap.dat
cd ../..
```
Finally, we can start our masternode daemon on the server to sync!
```
./ksocd -daemon
```
This should initiate the daemon based on if you have been following the guide up until this point.

After waiting a few moments we can check the sync process by using
```
./ksoc-cli getblockcount
```
cheers



