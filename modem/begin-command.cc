#include "modem/commands.h"

namespace firebase {
namespace modem {

bool BeginCommand::execute(const String& command,
                         InputStream* in, OutputStream* out) {
  if (in == nullptr || out == nullptr) {
    return false;
  }

  if (command != "BEGIN") {
    return false;
  }

  String host;
  String auth;

  String data(in->readLine());
  // Remove leading ' '.
  data = data.substring(1);

  int space_index = data.indexOf(' ');
  if (space_index == -1) {
    // host only.
    host = data;
  } else {
    // host and auth.
    host = data.substr(0, space_index);
    auth = data.substr(space_index + 1);
  }

  if (host.empty()) {
    out->println("-FAIL Missing host");
    return false;
  }

  new_firebase_.reset(new Firebase(host));
  if (!auth.empty()) {
    new_firebase_->auth(auth);
  }

  out->println("+OK");
  return true;
}

std::unique_ptr<Firebase> BeginCommand::firebase() {
  return std::move(new_firebase_);
}

}  // modem
}  // firebase
