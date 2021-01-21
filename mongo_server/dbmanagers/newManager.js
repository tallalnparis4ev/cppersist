class NewManager {
  constructor(database) {
      this.database = database;
  }
  setCollection(newCollection){
    if (newCollection != this.curCollection){
      this.collection = this.database.collection(newCollection);
      this.curCollection = newCollection;
    }
  }
  handlePost(request, response) {
    this.setCollection(request.params.function);
    const insert = {_id:request.params.id, return:request.body.return}
    this.collection.insertOne(insert, function(err, res) {
      if (err) console.log("error putting");
      response.sendStatus(200)
    });
  }
  handleGet(request, response){
    this.setCollection(request.params.function);
    this.collection.findOne({ "_id": request.params.id }, (error, result) => {
      if(error || (result == null)) 
        response.sendStatus(500);
      else
        response.send(result.return);
    });
  }
}
module.exports = NewManager
