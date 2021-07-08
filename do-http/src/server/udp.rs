use async_std::{
    task, fs,
    path::PathBuf,
    net::{TcpStream, UdpSocket, TcpListener},
    io::{self, Read, Write, ReadExt,  prelude::*},
    prelude::*
};

pub async fn udp() -> io::Result<()> {
    let socket = UdpSocket::bind("127.0.0.1:8080").await?;
    println!("Listening on {}", socket.local_addr()?);
    let mut buf = vec![0u8; 1024];
    loop {
        let (recv, peer) = socket.recv_from(&mut buf).await?;
        let sent = socket.send_to(&buf[..recv], &peer).await?;
        println!("Sent {} out of {} bytes to {}", sent, recv, peer);
    }
}
