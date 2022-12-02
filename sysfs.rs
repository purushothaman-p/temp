#![allow(warnings, unused)]
use std::fs::create_dir;
use std::fs::remove_file;
use std::fs::File;
use std::io::prelude::*;


pub struct kobject {
    pub name: String,
    pub driver_id: i32
}

impl kobject{
    fn show(&self){
        println!("Key Object Name: {}\nDriver ID: 0x{:x}\n", self.name, self.driver_id);
    }
    fn store(&mut self, attr:attribute){
        self.name = attr.name;
        self.driver_id = attr.id;
    }
}


pub struct attribute{
    pub name: String,
    pub id: i32
}

impl attribute{
    fn show(&self){
        println!("Attribute Name: {}\nAttribute ID: 0x{:x}\n", self.name, self.id);
    }
}

const fname : &'static str = "/dev/sys/sample-driver";
const EINVAL: i32 = 128; // some random constant for einval



fn kobject_create() -> bool{
    
    // should technically return a non-Null KeyObject pointer. returning bool to make things easy
    let mut kobj_ptr = true; // assuming memory allocation succeeds and returning a non-Null pointer
    /*
    let mut kobj :&kobject;
    kobj = malloc(sizeof(kobj)); -> no equivalent in rust
    if(!kobj)
        return false;
    kobject_init(kobj);
    */
    return kobj_ptr;
}

fn kobject_add(kobj: &kobject, parent: &kobject, fmt: &'static str, name: &'static str) -> i32{
    let mut retval:i32=0;
    /*
    if !kobj
        return -EINVAL;
    if !(kobj.state_initialized){
        println!("kobject {} {}: tried to add an uninitialized object, something is seriously wrong.\n", kobject.name, kobj)
        dump_stack();
        return -EINVAL;
    }
    va_start(name, fmt);
    retval = kobject_add_varg(kobj, parent, fmt, name);
    va_end(name);
    */
    return retval; // forcing it to zero and assuming key object is added successfully
}

pub fn kobject_create_and_add(name: &'static str, parent: &kobject) -> bool{
    let mut kobj_ptr: bool = true;
    let mut retval: i32 = 0;
    //kobj = kobj_create();
    if !kobj_ptr{
        return false; // should return null pointer
    }
    //retval = kobject_add(kobj, parent, "%s", name);
    if retval!=0 {
        println!("{} kobject_add error\n", retval);
        // kobject_put(kobj);
        kobj_ptr = false;
    }
    return kobj_ptr;
}

pub fn sysfs_create_file(ko : &kobject, attr: &attribute){
    sysfs_create_file_ns(ko, attr);
}

fn sysfs_create_file_ns(ko : &kobject, attr: &attribute){
    // write attributes into the file too
    let info = format!("Driver Name:  {}\n  Driver ID:  0x{:x}\n", ko.name, ko.driver_id);
    let mut file = File::create(fname).expect("error creating file\n");
    file.write_all(info.as_bytes()).expect("Error while writing to file\n");
}

pub fn sysfs_remove_file(ko : &kobject, attr: &attribute){
    sysfs_remove_file_ns(ko, attr);
}

fn sysfs_remove_file_ns(ko: &kobject, attr: &attribute){
    remove_file(fname).expect("Couldnt delete file\n");
}


fn main() {
    create_dir("/dev/sys/");
    /*
    let ko = kobject { driver_id: 35, name: "Sample Driver".to_string()};
    let attr = attribute {name: "attribute params".to_string(), id:45};
    let name = "Sample Sysfs\n";
    
    if kobject_create_and_add(name, &ko){
        sysfs_create_file(&ko, &attr);
        // sysfs_remove_file_ns(&ko, &attr);
    }
    println!("Hello World");
    */
}

