#strict 2
#appendto BLMP
#appendto BALN
#appendto TREE

public func GetVertexToConnect() {
  if(this -> ~IsTree())
  	return 2;
  if(GetID() == BLMP || GetID() == BALN)
  	return -1;
}
