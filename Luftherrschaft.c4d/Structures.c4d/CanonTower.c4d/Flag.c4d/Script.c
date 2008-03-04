/*-- Flagge --*/

#strict

func Initialize() {
  SetAction("Wind");
  return(1);
}

private func SetPos(){
  SetPosition(x,y,this());
  return(1);
}
