use std::{
    collections::HashMap,
    str::FromStr
};
use async_std::{
    task, fs,
    path::PathBuf,
    net::{TcpStream, UdpSocket, SocketAddrV4, TcpListener},
    io::{self, Read, Write, ReadExt,  prelude::*},
    prelude::*
};

#[derive(Debug, Clone)]
pub struct TcpServer {
    routes: Routes,
    addr: SocketAddrV4
}

impl TcpServer {
    pub async fn new(addr: &str) -> Self {
        let routes = vec![("/", "assets/html/index.html"), ("/auth", "assets/html/auth.html")];
        Self {
            routes: Routes::from_pages(routes),
            addr: SocketAddrV4::from_str(addr).unwrap(),
        }
    }
    pub async fn run(self) -> io::Result<()> {
        let listener = TcpListener::bind(self.addr).await?;
        println!("Listening on {}", listener.local_addr()?);
        let mut incoming = listener.incoming();
        while let Some(stream) = incoming.next().await {
            let stream = stream?;
            let r = vec![("/", "assets/html/index.html"), ("/auth", "assets/html/auth.html")];
            task::spawn(async {
                Routes::from_pages(r).handle(stream).await.unwrap();
            });
        }
        Ok(())
    }

}

#[derive(Debug, Clone)]
pub struct Routes(HashMap<String, String>);

impl Routes {
    pub async fn handle(self, stream: TcpStream) -> io::Result<()> {
        println!("Accepted from: {}", stream.peer_addr()?);
        let mut reader = stream.clone();
        let mut writer = stream;
        let resp = self.route(reader.clone()).await?;
        writer.write(resp.fmt().as_bytes()).await?;
        writer.flush().await?;
        io::copy(&mut reader, &mut writer).await?;
        Ok(())
    }
    pub fn from_pages(routes: Vec<(&'static str, &'static str)>) -> Self {
        let mut rt: HashMap<String, String> = HashMap::new();
        for (route, page) in routes.into_iter() {
            rt.insert(route.into(), page.into());
        }
        Self(rt)
    }
    pub async fn route(self, mut stream: TcpStream) -> io::Result<Resp> {
        let mut buf = [0; 1024];
        let res = stream.read(&mut buf).await;
        for route in  self.0.keys() {
            if buf.starts_with(Self::to_req(route).as_bytes()) {
                let page = self.0.get(route)
                    .map(|s| s.to_owned())
                    .unwrap_or_default();
                let resp: Resp = Resp::from_page(page.as_str()).await?;
                return Ok(resp);
            } else {
                continue
            }
        }
        return Ok(Resp::page_404(Some("assets/html/404.html".into())).await?);
    }
    pub fn to_req(route: &str) -> String {
        format!("GET {} HTTP/1.1\r\n", route)
    }
}
#[derive(Clone, Default, Debug)]
pub struct Resp {
    status: String,
    body: String,
}
impl Resp {

    pub fn new(status: String, body: String) -> Self {
        Self { status, body }
    }

    pub async fn from_page(page: &str) -> io::Result<Self> {
        Ok(Self {
            status: "HTTP/1.1 200 OK\r\n\r\n".into(),
            body: fs::read_to_string(page).await?,
        })

    }
    pub async fn page_404(page: Option<String>) -> io::Result<Self> {
        let body = if let Some(page) = page {
            fs::read_to_string(&page).await?
        } else {
            String::new()
        };
        Ok(Self {
            status: "HTTP/1.1 404 NOT FOUND\r\n\r\n".into(),
            body
        })
    }

    pub fn fmt(self) -> String {
        format!("{}{}", &self.status, &self.body)

    }

    /// Get a mutable reference to the resp's body.
    pub fn body_mut(&mut self) -> &mut String {
        &mut self.body
    }
}
