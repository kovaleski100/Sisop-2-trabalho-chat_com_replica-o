#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

struct Mensagem
{
    boost::uuids::uuid uuid;
    std::string grupo;
    std::string usuario;
    std::string texto;
    Mensagem()
    {
        uuid = boost::uuids::random_generator()();
    }
    Mensagem(std::string uuid, std::string grupo, std::string usuario, std::string texto)
    {
        boost::uuids::string_generator gen;
        this->uuid = gen(uuid);
        this->grupo = grupo;
        this->usuario = usuario;
        this->texto = texto;
    }
};
