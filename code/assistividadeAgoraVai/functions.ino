//Inicializa o sistema de arquivos.
bool initFS() {
  if (!SPIFFS.begin()) {
    Serial.println("Erro ao abrir o sistema de arquivos");
    return false;
  }
  Serial.println("Sistema de arquivos carregado com sucesso!");
  return true;
}

//Lista todos os arquivos salvos na flash.
void listAllFiles() {
  String str = "";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\r\n";
  }
  Serial.print(str);
}

//Faça a busca de um usuario pelo ID e pela INFO.
int findUser(vector <String> users_data, String id, String info) {
  String newID = "<td>" + id + "</td>";
  String newinfo = "<td>" + info + "</td>";
  for (int i = 0; i < users_data.size(); i++) {
    if (users_data[i].indexOf(newID) > 0 || users_data[i].indexOf(newinfo) > 0) {
      return i;
    }
  }
  return -1;
}

//Adiciona um novo usuario ao sistema
bool addNewUser(String id, String data) {
  File myFile = SPIFFS.open(FILENAME, "a+");
  if (!myFile) {
    Serial.println("Erro ao abrir arquivo!");
    myFile.close();
    return false;
  } else {
    myFile.printf("<tr><td>%s</td><td>%s</td>\n", id.c_str(), data.c_str());
    Serial.println("Arquivo gravado");
  }
  myFile.close();
  return true;
}

//Remove um usuario do sistema
bool removeUser(int user_index) {
  vector <String> users_data = readFile(FILENAME);
  if (user_index == -1)//Caso usuário não exista retorne falso
  return false;
  File myFile = SPIFFS.open(FILENAME, "w");
  if (!myFile) {
    Serial.println("Erro ao abrir arquivo!");
    myFile.close();
    return false;
  } else {
    for (int i = 0; i < users_data.size(); i++) {
      if (i != user_index)
      myFile.println(users_data[i]);
    }
    Serial.println("Usuário removido");
  }
  myFile.close();
  return true;
}

//Esta função substitui trechos de paginas html marcadas entre %
String processor(const String& var) {
  String msg = "";
  if (var == "TABLE") {
    msg = "<table><tr><td>RFID Code</td><td>User Info</td><td>Delete</td></tr>";
    vector <String> lines = readFile(FILENAME);
    for (int i = 0; i < lines.size(); i++) {
      msg += lines[i];
      msg += "<td><a href=\"get?remove=" + String(i + 1) + "\"><button>Excluir</button></a></td></tr>"; //Adiciona um botão com um link para o indice do usuário na tabela
    }
    msg += "</table>";
  }
  else if (var == "SUCESS_MSG")
  msg = sucess_msg;
  else if (var == "FAILURE_MSG")
  msg = failure_msg;
  return msg;
}
