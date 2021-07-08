pub mod tcp;
pub mod udp;
use std::collections::HashMap;
use async_std::{
    task, fs,
    path::PathBuf,
    net::{TcpStream, UdpSocket, TcpListener},
    io::{self, Read, Write, ReadExt,  prelude::*},
    prelude::*
};

#[derive(Debug)]
pub struct Server<C> {
    static_dir: PathBuf,
    routes: Vec<String>,
    context: Option<C>
}




#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}
