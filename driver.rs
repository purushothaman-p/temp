mod sysfs;

fn main() {
    let ko = sysfs::kobject { driver_id: 35, name: "Sample Driver".to_string()};
    let attr = sysfs::attribute {name: "attribute params".to_string(), id:45};
    let name = "Sample Sysfs\n";
    
    if sysfs::kobject_create_and_add(name, &ko){
        sysfs::sysfs_create_file(&ko, &attr);
        // sysfs_remove_file_ns(&ko, &attr);
    }
    println!("Hello World");
}

