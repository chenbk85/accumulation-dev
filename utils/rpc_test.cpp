#include <stdio.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <tuple>

#include "json_object.h"

using namespace std;

template <typename T>
class HasCallOperator
{
    typedef char _One;
    typedef struct{ char a[2]; }_Two;
    template<typename T>
    static _One hasFunc(decltype(&T::operator()));
    template<typename T>
    static _Two hasFunc(...);
public:
    static const bool value = sizeof(hasFunc<T>(nullptr)) == sizeof(_One);
};

namespace dodo
{
    class Utils
    {
    public:
        template<typename U, typename V>
        void static readJson(JsonObject& msg, const char* key, map<U, V>& tmp)
        {
            /*����map������msg�е�key����ȡmap��������Ӧ��jsonobject*/
            JsonObject mapObject = msg.getObject(key);
            /*������map��jsonobject*/
            for (Json::Value::const_iterator it = mapObject.begin(); it != mapObject.end(); ++it)
            {
                /*���ݴ�������key����map��jsonobject���ȡ��Ӧ��value*/
                V tv;
                Json::Value vkey = it.key();
                readJson(mapObject, vkey.asString().c_str(), tv);

                /*��json�е�key(����string)ת������ʵ��key(int��string)*/
                U realKey;
                stringstream ss;
                ss << vkey.asString();
                ss >> realKey;

                /*��value���뵽���map��*/
                tmp[realKey] = tv;
            }    
        }

        void static readJson(JsonObject& msg, const char* key, char& tmp)
        {
            tmp = msg.getInt(key);
        }

        void static readJson(JsonObject& msg, const char* key, int& tmp)
        {
            tmp = msg.getInt(key);
        }

        void static readJson(JsonObject& msg, const char* key, JsonObject& tmp)
        {
            tmp = msg.getObject(key);
        }

        void static readJson(JsonObject& msg, const char* key, string& tmp)
        {
            tmp = msg.getStr(key);
        }

        void static readJson(JsonObject& msg, const char* key, vector<int>& tmp)
        {
            vector<int> ret;
            JsonObject arrayJson = msg.getObject(key);
            for (int i = 0; i < arrayJson.getSize(); ++i)
            {
                stringstream ss;
                ss << i;
                JsonObject valueObject = arrayJson.getByIndex(i);
                ret.push_back(atoi(valueObject.toString().c_str()));
            }
            tmp = ret;
        }

        void static readJson(JsonObject& msg, const char* key, vector<string>& tmp)
        {
            vector<string> ret;
            JsonObject arrayJson = msg.getObject(key);
            for (int i = 0; i < arrayJson.getSize(); ++i)
            {
                stringstream ss;
                ss << i;
                JsonObject valueObject = arrayJson.getByIndex(i);
                ret.push_back(valueObject.getJsonValue().asString());
            }
            tmp = ret;
        }

        void static readJson(JsonObject& msg, const char* key, map<string, string>& tmp)
        {
            map<string, string> ret;
            JsonObject mapJson = msg.getObject(key);
            Json::Value::const_iterator itend = mapJson.end();
            for (Json::Value::const_iterator it = mapJson.begin(); it != itend; ++it)
            {
                Json::Value vkey = it.key();
                Json::Value vvalue = *it;
                ret[vkey.asString()] = vvalue.asString();
            }
            tmp = ret;
        }

        void static readJson(JsonObject& msg, const char* key, map<int, string>& tmp)
        {
            map<int, string> ret;
            JsonObject mapJson = msg.getObject(key);
            Json::Value::const_iterator itend = mapJson.end();
            for (Json::Value::const_iterator it = mapJson.begin(); it != itend; ++it)
            {
                Json::Value vkey = it.key();
                Json::Value vvalue = *it;
                ret[atoi(vkey.asString().c_str())] = vvalue.asString();
            }
            tmp = ret;
        }

        void static readJson(JsonObject& msg, const char* key, map<string, int>& tmp)
        {
            map<string, int> ret;
            JsonObject mapJson = msg.getObject(key);
            Json::Value::const_iterator itend = mapJson.end();
            for (Json::Value::const_iterator it = mapJson.begin(); it != itend; ++it)
            {
                Json::Value vkey = it.key();
                Json::Value vvalue = *it;
                ret[vkey.asString()] = vvalue.asInt();
            }
            tmp = ret;
        }

        template<typename T>
        T static readJsonByIndex(JsonObject& msg, int index)
        {
            stringstream ss;
            ss << index;
            T tmp;
            readJson(msg, ss.str().c_str(), tmp);
            return tmp;
        }

    public:
        void    static  writeJson(JsonObject& msg, int value, const char* key)
        {
            msg.setInt(key, value);
        }

        void    static  writeJson(JsonObject& msg, const char* value, const char* key)
        {
            msg.setStr(key, value);
        }

        void    static  writeJson(JsonObject& msg, string value, const char* key)
        {
            msg.setStr(key, value.c_str());
        }

        void    static  writeJson(JsonObject& msg, JsonObject value, const char* key)
        {
            msg.setObject(key, value);
        }

        void    static  writeJson(JsonObject& msg, vector<int> value, const char* key)
        {
            JsonObject arrayObject;
            for (size_t i = 0; i < value.size(); ++i)
            {
                arrayObject.appendInt(value[i]);
            }
            msg.setObject(key, arrayObject);
        }

        template<typename T, typename V>
        void    static    writeJson(JsonObject& msg, map<T, V> value, const char* key)
        {
            JsonObject mapObject;
            /*������map*/
            for (map<T, V>::iterator it = value.begin(); it != value.end(); ++it)
            {
                stringstream ss;
                ss << it->first;
                /*��value���л���map��jsonobject��,key��������map�ṹ�е�key*/
                writeJson(mapObject, it->second, ss.str().c_str());
            }

            /*�Ѵ�map���ӵ�msg��*/
            msg.setObject(key, mapObject);
        }

        void    static  writeJson(JsonObject& msg, vector<string> value, const char* key)
        {
            JsonObject arrayObject;
            for (size_t i = 0; i < value.size(); ++i)
            {
                arrayObject.appendStr(value[i].c_str());
            }
            msg.setObject(key, arrayObject);
        }

        void    static  writeJson(JsonObject& msg, map<string, string> value, const char* key)
        {
            JsonObject mapObject;
            map<string, string>::iterator itend = value.end();
            for (map<string, string>::iterator it = value.begin(); it != itend; ++it)
            {
                mapObject.setStr(it->first.c_str(), it->second.c_str());
            }
            msg.setObject(key, mapObject);
        }

        void    static  writeJson(JsonObject& msg, map<int, string> value, const char* key)
        {
            JsonObject mapObject;
            map<int, string>::iterator itend = value.end();
            for (map<int, string>::iterator it = value.begin(); it != itend; ++it)
            {
                stringstream ss;
                ss << it->first;
                mapObject.setStr(ss.str().c_str(), it->second.c_str());
            }
            msg.setObject(key, mapObject);
        }

        void    static  writeJson(JsonObject& msg, map<string, int> value, const char* key)
        {
            JsonObject mapObject;
            map<string, int>::iterator itend = value.end();
            for (map<string, int>::iterator it = value.begin(); it != itend; ++it)
            {
                mapObject.setInt(it->first.c_str(), it->second);
            }
            msg.setObject(key, mapObject);
        }

        template<typename T>
        void    static  writeJsonByIndex(JsonObject& msg, T t, int index)
        {
            stringstream ss;
            ss << index;
            writeJson(msg, t, ss.str().c_str());
        }
    };

    class LambdaMgr
    {
    public:
        void    callLambda(const char* str)
        {
            JsonObject msgObject;
            msgObject.read(str);

            int id = msgObject.getInt("id");
            JsonObject parmObject = msgObject.getObject("parm");

            assert(mWrapFunctions.find(id) != mWrapFunctions.end());
            if (mWrapFunctions.find(id) != mWrapFunctions.end())
            {
                mWrapFunctions[id](mRealLambdaPtr[id], parmObject.toString().c_str());
            }
        }

        template<typename T>
        void insertLambda(T lambdaObj)
        {
            _insertLambda(mNextID, lambdaObj, &T::operator());
            mNextID++;
        }
    private:
        template<typename RVal, typename T, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void, typename T6 = void, typename T7 = void>
        struct LambdaFunctor
        {
            static int invoke(void* pvoid, const char* str)
            {
            }
        };

        template<typename RVal, typename T, typename T1, typename T2, typename T3, typename T4>
        struct LambdaFunctor<RVal, T, T1, T2, T3, T4>
        {
        public:
            LambdaFunctor(std::function<void(T1, T2, T3, T4)> f)
            {
                mf = f;
            }

            static void invoke(void* pvoid, const char* str)
            {
                JsonObject jsonObject;
                jsonObject.read(str);

                int parmIndex = 0;
                T1 parm1 = Utils::readJsonByIndex<T1>(jsonObject, parmIndex++);
                T2 parm2 = Utils::readJsonByIndex<T2>(jsonObject, parmIndex++);
                T3 parm3 = Utils::readJsonByIndex<T3>(jsonObject, parmIndex++);
                T4 parm4 = Utils::readJsonByIndex<T4>(jsonObject, parmIndex++);

                LambdaFunctor<RVal, T, T1, T2, T3, T4>* pthis = (LambdaFunctor<RVal, T, T1, T2, T3, T4>*)pvoid;
                (pthis->mf)(parm1, parm2, parm3, parm4);
            }

        private:
            std::function<void(T1, T2, T3, T4)>   mf;
        };

        template<typename RVal, typename T, typename T1, typename T2, typename T3>
        struct LambdaFunctor<RVal, T, T1, T2, T3>
        {
        public:
            LambdaFunctor(std::function<void(T1, T2, T3)> f)
            {
                mf = f;
            }

            static void invoke(void* pvoid, const char* str)
            {
                JsonObject jsonObject;
                jsonObject.read(str);

                int parmIndex = 0;
                T1 parm1 = Utils::readJsonByIndex<T1>(jsonObject, parmIndex++);
                T2 parm2 = Utils::readJsonByIndex<T2>(jsonObject, parmIndex++);
                T3 parm3 = Utils::readJsonByIndex<T3>(jsonObject, parmIndex++);

                LambdaFunctor<RVal, T, T1, T2, T3>* pthis = (LambdaFunctor<RVal, T, T1, T2, T3>*)pvoid;
                (pthis->mf)(parm1, parm2, parm3);
            }

        private:
            std::function<void(T1, T2, T3)>   mf;
        };

        template<typename RVal, typename T, typename T1, typename T2>
        struct LambdaFunctor<RVal, T, T1, T2>
        {
        public:
            LambdaFunctor(std::function<void(T1, T2)> f)
            {
                mf = f;
            }

            static void invoke(void* pvoid, const char* str)
            {
                JsonObject jsonObject;
                jsonObject.read(str);

                int parmIndex = 0;
                T1 parm1 = Utils::readJsonByIndex<T1>(jsonObject, parmIndex++);
                T2 parm2 = Utils::readJsonByIndex<T2>(jsonObject, parmIndex++);

                LambdaFunctor<RVal, T, T1, T2>* pthis = (LambdaFunctor<RVal, T, T1, T2>*)pvoid;
                (pthis->mf)(parm1, parm2);
            }

        private:
            std::function<void(T1, T2)>   mf;
        };

        template<typename RVal, typename T, typename T1>
        struct LambdaFunctor<RVal, T, T1>
        {
        public:
            LambdaFunctor(std::function<void(T1)> f)
            {
                mf = f;
            }

            static void invoke(void* pvoid, const char* str)
            {
                JsonObject jsonObject;
                jsonObject.read(str);

                int parmIndex = 0;
                T1 parm1 = Utils::readJsonByIndex<T1>(jsonObject, parmIndex++);

                LambdaFunctor<RVal, T, T1>* pthis = (LambdaFunctor<RVal, T, T1>*)pvoid;
                (pthis->mf)(parm1, parm2);
            }
        private:
            std::function<void(T1)>   mf;
        };

        template<typename RVal, typename T>
        struct LambdaFunctor<RVal, T>
        {
        public:
            LambdaFunctor(std::function<void(void)> f)
            {
                mf = f;
            }

            static void invoke(void* pvoid, const char* str)
            {
                LambdaFunctor<RVal, T>* pthis = (LambdaFunctor<RVal, T>*)pvoid;
                (pthis->mf)();
            }

        private:
            std::function<void(void)>   mf;
        };

        template<typename LAMBDA_OBJ_TYPE, typename RVal>
        void _insertLambda(int iid, LAMBDA_OBJ_TYPE obj, RVal(LAMBDA_OBJ_TYPE::*func)() const)
        {
            void* pbase = new LambdaFunctor<void, LAMBDA_OBJ_TYPE>(obj);

            mWrapFunctions[mNextID] = LambdaFunctor<void, LAMBDA_OBJ_TYPE>::invoke;
            mRealLambdaPtr[mNextID] = pbase;
        }


        template<typename LAMBDA_OBJ_TYPE, typename RVal, typename T1>
        void _insertLambda(int iid, LAMBDA_OBJ_TYPE obj, RVal(LAMBDA_OBJ_TYPE::*func)(T1) const)
        {
            void* pbase = new LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1>(obj);

            mWrapFunctions[mNextID] = LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1>::invoke;
            mRealLambdaPtr[mNextID] = pbase;
        }

        template<typename LAMBDA_OBJ_TYPE, typename RVal, typename T1, typename T2>
        void _insertLambda(int iid, LAMBDA_OBJ_TYPE obj, RVal(LAMBDA_OBJ_TYPE::*func)(T1, T2) const)
        {
            void* pbase = new LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1, T2>(obj);

            mWrapFunctions[mNextID] = LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1, T2>::invoke;
            mRealLambdaPtr[mNextID] = pbase;
        }

        template<typename LAMBDA_OBJ_TYPE, typename RVal, typename T1, typename T2, typename T3>
        void _insertLambda(int iid, LAMBDA_OBJ_TYPE obj, RVal(LAMBDA_OBJ_TYPE::*func)(T1, T2, T3) const)
        {
            void* pbase = new LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1, T2, T3>(obj);

            mWrapFunctions[mNextID] = LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1, T2, T3>::invoke;
            mRealLambdaPtr[mNextID] = pbase;
        }

        template<typename LAMBDA_OBJ_TYPE, typename RVal, typename T1, typename T2, typename T3, typename T4>
        void _insertLambda(int iid, LAMBDA_OBJ_TYPE obj, RVal(LAMBDA_OBJ_TYPE::*func)(T1, T2, T3, T4) const)
        {
            void* pbase = new LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1, T2, T3, T4>(obj);

            mWrapFunctions[mNextID] = LambdaFunctor<void, LAMBDA_OBJ_TYPE, T1, T2, T3, T4>::invoke;
            mRealLambdaPtr[mNextID] = pbase;
        }
    private:
        int     mNextID;

        typedef void(*pf_wrap)(void* pbase, const char* parmStr);
        map<int, pf_wrap>       mWrapFunctions;
        map<int, void*>         mRealLambdaPtr;
    };

    template<bool>
    struct SelectWriteArg;

    template<>
    struct SelectWriteArg<true>
    {
        template<typename ARGTYPE>
        static  void    Write(LambdaMgr& lambdaMgr, JsonObject& parms, ARGTYPE arg, int index)
        {
            lambdaMgr.insertLambda(arg);
        }
    };

    template<>
    struct SelectWriteArg<false>
    {
        template<typename ARGTYPE>
        static  void    Write(LambdaMgr& lambdaMgr, JsonObject& parms, ARGTYPE arg, int index)
        {
            Utils::writeJsonByIndex(parms, arg, index);
        }
    };

    class rpc
    {
    public:
        template<typename F>
        void        def(const char* funname, F func)
        {
            regFunctor(funname, func);
        }

        void    writeCallArg(JsonObject& msg, int& index){}

        template<typename Arg>
        void    writeCallArg(JsonObject& msg, int& index, const Arg& arg)
        {
            /*ֻ(ʣ)��һ������,�϶�ҲΪ���һ������������Ϊlambda*/
            _selectWriteArg(msg, arg, index++);
        }

        template<typename Arg1, typename... Args>
        void    writeCallArg(JsonObject& msg, int& index, const Arg1& arg1, const Args&... args)
        {
            Utils::writeJsonByIndex(msg, arg1, index++);
            writeCallArg(msg, index, args...);
        }

        template<typename... Args>
        void    call(const char* funname, const Args&... args)
        {
            JsonObject msg;
            msg.setStr("name", funname);

            JsonObject parms;
            int index = 0;
            writeCallArg(parms, index, args...);

            msg.setObject("parm", parms);

            handleMsg(msg.toString().c_str());
        }

        void    callLambda(const char* str)
        {
            mLambdaMgr.callLambda(str);
        }

        void handleMsg(const char* str)
        {
            JsonObject msg;
            msg.read(str);

            string funname = msg.getStr("name");
            JsonObject parm = msg.getObject("parm");
            assert(mWrapFunctions.find(funname) != mWrapFunctions.end());
            if (mWrapFunctions.find(funname) != mWrapFunctions.end())
            {
                mWrapFunctions[funname](mRealFunctions[funname], parm.toString().c_str());
            }
        }

    private:

        /*�����lambda�����ص����������������ӵ�rpc����*/
        template<typename ARGTYPE>
        void    _selectWriteArg(JsonObject& parms, ARGTYPE arg, int index)
        {
            SelectWriteArg<HasCallOperator<ARGTYPE>::value>::Write(mLambdaMgr, parms, arg, index);
        }

    private:
        template<typename RVal, typename ...Args>
        struct VariadicFunctor
        {
            static  void    invoke(void* realfunc, const char* str)
            {
                JsonObject msg;
                msg.read(str);
                int parmIndex = 0;  /*parmIndex��Ϊjson��ÿ��������key������*/
                eval<Args...>(SizeType<sizeof...(Args)>::TYPE(), realfunc, msg, parmIndex);
            }

            template<int Size>
            struct SizeType
            {
                typedef int TYPE;
            };
            template<>
            struct SizeType<0> 
            {
                typedef char TYPE;
            };

            template<typename T, typename ...LeftArgs, typename ...NowArgs>
            static  void    eval(int _, void* realfunc, JsonObject& msg, int& parmIndex, const NowArgs&... args)
            {
                /*argsΪ�Ѿ���ֵ�Ĳ����б�*/
                /*cur_argΪ������ֵ�Ĳ���*/
                T cur_arg = Utils::readJsonByIndex<T>(msg, parmIndex++);
                eval<LeftArgs...>(SizeType<sizeof...(LeftArgs)>::TYPE(), realfunc, msg, parmIndex, args..., cur_arg);
            }

            template<typename ...NowArgs>
            static  void    eval(char _, void* realfunc, JsonObject& msg, int& parmIndex, const NowArgs&... args)
            {
                /*û���κ�ʣ�µ�δ֪�������ͣ���ô args �βξ������յ����в������ڴ˻ص���������*/
                typedef void(*pf)(NowArgs...);
                pf p = (pf)realfunc;
                p(args...);
            }

        private:
        };

        template<typename RVal, typename ...Args>
        void regFunctor(const char* funname, RVal(*func)(Args...))
        {
            mWrapFunctions[funname] = VariadicFunctor<RVal, Args...>::invoke;
            mRealFunctions[funname] = func;
        }
    private:
        typedef void(*pf_wrap)(void* realFunc, const char* parmStr);
        map<string, pf_wrap>    mWrapFunctions; /*  ��װ������   */
        map<string, void*>      mRealFunctions; /*  ��ʵ������   */
        LambdaMgr               mLambdaMgr;
    };

    template struct SelectWriteArg<true>;
    template struct SelectWriteArg<false>;
}

void test1(int a, int b)
{
    cout << "in test1" << endl;
    cout << a << ", " << b << endl;
}

void test2(int a, int b, string c)
{
    cout << "in test2" << endl;
    cout << a << ", " << b << ", " << c << endl;
}

void test3(string a, int b, string c)
{
    cout << "in test3" << endl;
    cout << a << ", " << b << ", " << c << endl;
}

void test4(string a, int b)
{
    cout << "in test4" << endl;
    cout << a << "," << b <<  endl;
}

void test5(string a, int b, map<int, map<int, string>> vlist)
{
}

void test6(string a, int b, map<string, int> vlist)
{
}

void test7()
{}

int main()
{
    int upvalue = 10;
    using namespace dodo;
    rpc rpc;
    rpc.def("test4", test4);
    rpc.def("test5", test5);
    rpc.def("test7", test7);

    map<int, string> t1;
    t1[1] = "Li";

    map<int, string> t2;
    t2[2] = "Deng";

    map<int, map<int, string>> vlist;

    vlist[100] = t1;
    vlist[200] = t2;

    rpc.call("test7");

    /*����Զ�̺���,������lambda�ص�����*/
    rpc.call("test5", "a", 1, vlist, [&upvalue](int a, int b){
        upvalue++;
        cout << "upvalue:" << upvalue << ", a:" << a << ", b:" << b << endl;
    });
    rpc.call("test5", "a", 1, vlist, [&upvalue](string a, string b, int c){
        upvalue++;
        cout << "upvalue:" << upvalue << ", a:" << a << ", b:" << b << ", c:" << c << endl;
    });
    /*��lambda�ص�*/
    rpc.call("test4", "a", 1);

    /*ģ��(�����÷�)�������÷���lambda����*/
    {
        JsonObject msg;
        msg.setInt("id", 0);        /*����Զ�̵��õ�lambda id*/

        {
            JsonObject parm;        /*����Զ�̵��õĲ���*/
            parm.setInt("0", 1);
            parm.setInt("1", 2);
            msg.setObject("parm", parm);
        }

        /*ģ����յ������÷���reply��Ϣ(rpc����ֵ)������������lambda����(����rpc����ֵ)*/
        rpc.callLambda(msg.toString().c_str());
    }

    {
        JsonObject msg;
        msg.setInt("id", 1);

        {
            JsonObject parm;
            parm.setStr("0", "hello");
            parm.setStr("1", "world");
            parm.setInt("2", 3);
            msg.setObject("parm", parm);
        }
        rpc.callLambda(msg.toString().c_str());
    }

    cin.get();
    return 0;
}