#include "../inc/server.hpp"
#include "../bonus/CBot.hpp"
#include "../inc/utils.hpp"

// bot

size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     std::string *userp) {
  userp->append((char *)contents, size * nmemb);
  return size * nmemb;
}

UserGit parccing(std::string Buffer) {
  UserGit user;
  std::string key;
  std::string val;
  std::map<std::string, std::string>::iterator it;
  int i = 0;

  Buffer.erase(0, 1);
  Buffer.erase(Buffer.length() - 1, 1);

  std::map<std::string, std::string> info;
  std::istrstream buff(Buffer.c_str());
  std::string line;

  while (std::getline(buff, line, ',')) {
    i = line.find(':');
    if (i > 0) {
      key = line.substr(0, i);
      val = line.substr(i, line.length());
      info[key] = val;
    }
  }
  it = info.find("\"message\"");
  if (it != info.end()) {
    user.existe = false;
    return user;
  }
  user.existe = true;
  user.name = info["\"login\""];
  user.bio = info["\"bio\""];
  user.followers = info["\"followers\""];
  user.following = info["\"following\""];
  user.public_repo = info["\"public_repos\""];
  user.dateCreation = info["\"created_at\""];
  return user;
}

void Server::botReseveMsg(int fd, std::string msg) {
  UserGit user;
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  std::string url;
  Client *cli = getClient(fd);

  url = "https://api.github.com/users/" + msg;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "bot");
    curl_easy_setopt(curl, CURLOPT_USERPWD, "username:token");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (CURLE_OK != res) {
      std::cerr << "cURL error: " << res << '\n';
      replies(fd, RPL_PRIVMSGCHANNEL(cli->getNickname(), cli->getUsername(),
                                     cli->getIpadd(), cli->getNickname(),
                                     ":User not found!"));
      return;
    } else {
      std::cout << "~~~~ Succeed ~~~" << std::endl;
    }
  }
  user = parccing(readBuffer);

  Client* bot = getClient("bot");

  if (user.existe == true) {
    replies(fd, RPL_PRIVMSGCHANNEL(bot->getNickname(), bot->getUsername(),
                                   bot->getIpadd(), cli->getNickname(),
                                   user.name + " create this accout at " +
                                       user.dateCreation));
    replies(fd, RPL_PRIVMSGCHANNEL(bot->getNickname(), bot->getUsername(),
                                   bot->getIpadd(), cli->getNickname(),
                                   ":Bio " + user.bio));
    replies(fd, RPL_PRIVMSGCHANNEL(bot->getNickname(), bot->getUsername(),
                               bot->getIpadd(), cli->getNickname(),
                               ":Number of Followers    " + user.followers));
    replies(fd, RPL_PRIVMSGCHANNEL(bot->getNickname(), bot->getUsername(),
                               bot->getIpadd(), cli->getNickname(),
                               ":Number of Following    " + user.following));
    replies(fd, RPL_PRIVMSGCHANNEL(bot->getNickname(), bot->getUsername(),
                               bot->getIpadd(), cli->getNickname(),
                               ":Number of public repos " + user.public_repo));
  } else {
    replies(fd, RPL_PRIVMSGCHANNEL(bot->getNickname(), bot->getUsername(),
                                   bot->getIpadd(), cli->getNickname(),
                                   ":User not found!"));
  }
}
