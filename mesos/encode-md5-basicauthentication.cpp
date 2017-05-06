// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License

#include <process/authenticator.hpp>
#include <process/id.hpp>

#include <string>
#include <vector>

#include <process/dispatch.hpp>
#include <process/future.hpp>
#include <process/http.hpp>
#include <process/process.hpp>

#include <stout/base64.hpp>
#include <stout/hashmap.hpp>
#include <stout/option.hpp>
#include <stout/strings.hpp>
#include <stout/try.hpp>

#include "../md5/md5_generator_token.cpp"

namespace process {
namespace http {
namespace authentication {

using std::string;
using std::vector;
class EncodeMD5BasicAuthenticatorProcess;

class EncodeMD5BasicAuthenticator : public Authenticator
{
public:
  EncodeMD5BasicAuthenticator(
      const std::string& realm,
      const hashmap<std::string, std::string>& credentials);

  virtual ~EncodeMD5BasicAuthenticator();

  virtual Future<AuthenticationResult> authenticate(
      const http::Request& request) override;

  virtual std::string scheme() const override;

private:
  Owned<EncodeMD5BasicAuthenticatorProcess> process_;
};


class EncodeMD5BasicAuthenticatorProcess : public Process<EncodeMD5BasicAuthenticatorProcess>
{
public:
  EncodeMD5BasicAuthenticatorProcess(
      const string& realm,
      const hashmap<string, string>& credentials);

  virtual Future<AuthenticationResult> authenticate(
      const http::Request& request);

private:
  const string realm_;
  const hashmap<string, string> credentials_;
};


EncodeMD5BasicAuthenticatorProcess::EncodeMD5BasicAuthenticatorProcess(
    const string& realm,
    const hashmap<string, string>& credentials)
  : ProcessBase(ID::generate("__basic_authenticator__")),
    realm_(realm),
    credentials_(credentials) {}


Future<AuthenticationResult> EncodeMD5BasicAuthenticatorProcess::authenticate(
    const Request& request)
{
  AuthenticationResult unauthorized;
  unauthorized.unauthorized =
    Unauthorized({"Basic realm=\"" + realm_ + "\""});

  Option<string> credentials = request.headers.get("Authorization");

  if (credentials.isNone()) {
    return unauthorized;
  }

  vector<string> components = strings::split(credentials.get(), " ");

  if (components.size() != 2 || components[0] != "Basic") {
    return unauthorized;
  }

  Try<string> decoded = base64::decode(components[1]);

  if (decoded.isError()) {
    return unauthorized;
  }
  LOG(INFO)<<"======================================";
  LOG(INFO)<<decoded.get();
  LOG(INFO)<<"======================================";

  vector<string> credential = strings::split(decoded.get(), ":");

  GeneratorMD5 md5(credential[1]);

  if (credential.size() != 2 ||
      !credentials_.contains(credential[0]) ||
      credentials_.at(credential[0]) != md5.render()) {
    return unauthorized;
  }

  AuthenticationResult authenticated;
  authenticated.principal = credential[0];
  return authenticated;
}


EncodeMD5BasicAuthenticator::EncodeMD5BasicAuthenticator(
    const string& realm,
    const hashmap<string, string>& credentials)
  : process_(new EncodeMD5BasicAuthenticatorProcess(realm, credentials))
{
  spawn(*process_);
}


EncodeMD5BasicAuthenticator::~EncodeMD5BasicAuthenticator()
{
  terminate(*process_);
  wait(*process_);
}


Future<AuthenticationResult> EncodeMD5BasicAuthenticator::authenticate(
    const Request& request)
{
  return dispatch(*process_, &EncodeMD5BasicAuthenticatorProcess::authenticate, request);
}


string EncodeMD5BasicAuthenticator::scheme() const
{
  return "EncodeMD5Basic";
}


} // namespace authentication {
} // namespace http {
} // namespace process {
