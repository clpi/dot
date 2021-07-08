pub mod script;
pub mod step;

pub use script::Script;
pub use step::Step;

#[derive(Debug, Clone)]
pub struct Task<D> {
    id: usize,
    data: D,
}

impl<D> Task<D> {

}
