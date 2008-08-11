/*-- Informationicon --*/

#strict 2

func Initialize() {
  SetVisibility(VIS_Owner);
  return 1;
}

public func Set(string graphics) {
  SetGraphics(graphics, this, GetID(this), 0, 1);
}

public func SetStatusMessage(string msg, int iLines) {
  CustomMessage(msg, this, GetOwner(), 30, 60 + iLines * 95 / 10); // jede Zeile hat 9,5px
}

public func BuildMessage(int iNeededMaterial, int iMaterial) {
	var szMessage = Format("@%d|<c ", iMaterial);
	if(iNeededMaterial > iMaterial)
		Add(szMessage, "ff0000");
	else
		Add(szMessage, "00ff00");
	Add(szMessage, Format(">%d</c>", iNeededMaterial));
	SetStatusMessage(szMessage, 2);
	return 1;
}

private func Add(string &szString1, string szString2) {
	return szString1 = Format("%s%s", szString1, szString2);
}
