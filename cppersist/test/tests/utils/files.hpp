void removeDir(string dir){
  std::filesystem::remove_all(dir);
}