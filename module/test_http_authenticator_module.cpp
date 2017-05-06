// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <mesos/mesos.hpp>
#include <mesos/module.hpp>

#include <mesos/module/http_authenticator.hpp>

#include <process/authenticator.hpp>

#include <stout/hashmap.hpp>

#include "../mesos/encode-md5-basicauthentication.cpp"

using namespace mesos;

using std::string;

using process::http::authentication::Authenticator;

using process::http::authentication::BasicAuthenticator;

using process::http::authentication::EncodeMD5BasicAuthenticator;

static Authenticator* createHttpAuthenticator(const Parameters& parameters)
{
   LOG(INFO)<<"Load createHttpAuthenticator function.";

   hashmap<string, string> credentialMap;
   credentialMap.put("user1", "e10adc3949ba59abbe56e057f20f883e");

   Authenticator* authenticator = new EncodeMD5BasicAuthenticator("mesos-master-readonly", credentialMap);
   return authenticator;  
}


// Declares an HTTP Authenticator module named
// 'org_apache_mesos_TestHttpBasicAuthenticator'.
mesos::modules::Module<Authenticator>
org_apache_mesos_TestHttpBasicAuthenticator(
    MESOS_MODULE_API_VERSION,
    MESOS_VERSION,
    "Apache Mesos",
    "modules@mesos.apache.org",
    "Test HTTP Authenticator module.",
    nullptr,
    createHttpAuthenticator);
