## Compiler module command:
```
# cd mesos-authentication-md5-module/module
# g++ -std=gnu++11 -lmesos -fpic -c test_authentication_modules.cpp test_
http_authenticator_module.cpp
```

## Shared library wrapper command:
```
# g++ -shared -lmesos -o libauthen.so test_authentication_modules.o test_http_authenticator_module.o
```

## Start mesos master command:
```
# mesos-master --ip=192.168.1.95 --work_dir=/tmp/mesos --acls=file:///home/user1/auth/acl 
--authenticate_http_readonly 
--authenticate_agents 
--credentials=file:///home/user1/auth/credentials --modules=file:///root/module.json --http_authenticators=org_apache_mesos_TestHttpBasicAuthenticator
```

## Start mesos agent command:
```
# mesos-agent --master=192.168.1.95:5050 
--work_dir=/tmp/mesos 
--credential=/root/auth/agent_credential --authenticatee=org_apache_mesos_TestCRAMMD5Authenticatee --modules=file:///root/module.json
```

## Setting Environment Variable in bash:
```
# export MESOS_AUTHENTICATEE=org_apache_mesos_TestCRAMMD5Authenticatee
# export MESOS_MODULES=file:///root/module.json
# export MESOS_NATIVE_JAVA_LIBRARY=/usr/local/lib/libmesos.so
```

## Start marathon framework command:
```
# $MARATHON_FRAMEWORK_HOME/bin/start --master=192.168.1.95:5050 --mesos_authentication 
--mesos_authentication_principal user1 
--mesos_authentication_secret secret
```

