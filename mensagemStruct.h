#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

struct mensagem
{
    boost::uuids::uuid uuid;
    int grupo;
    std::string usuario;
    std::string texto;
    mensagem()
    {
        uuid = boost::uuids::random_generator()();
    }
};
