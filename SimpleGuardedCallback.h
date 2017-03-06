#include <functional>
#include <memory>

namespace _Internal
{
    template<typename ... TArgs>
    using TokenizedFunction = std::function<void(unsigned int token, const TArgs&...)> ;

    template<typename ... TArgs>
    using Function = std::function<void(const TArgs&...)> ;

    template<typename ... TArgs>
    void guardedFunction(std::weak_ptr<bool> flag, TokenizedFunction<TArgs...> function, unsigned int token, const TArgs&... args)
    {
        if(!flag.expired())
        {
            function(token, args...);
        }
    }

    template<std::size_t N, typename ... TArgs>
    struct Guarder
    {
        static void validate()
        {
            static_assert((sizeof...(TArgs)) == N, "Amount of function arguments has to be equal to N template parameter");
        }

        static Function<TArgs...> createGuardedFunction(std::weak_ptr<bool> flag, TokenizedFunction<TArgs...> function, unsigned int token);
    };
    //Function<TArgs...> createGuardedFunction(std::weak_ptr<bool> flag, Function<TArgs...> function, unsigned int token);

    template <typename ... TArgs>
    struct Guarder<1, TArgs...>
    {
        static Function<TArgs...> createGuardedFunction(std::weak_ptr<bool> flag, TokenizedFunction<TArgs...> function, unsigned int token)
        {
            if(function != nullptr)
            {
                return std::bind(_Internal::guardedFunction<TArgs...>, flag, function, token, std::placeholders::_1);
            }

            return nullptr;
        }
    };

    template <typename ... TArgs>
    struct Guarder<2, TArgs...>
    {
        static Function<TArgs...> createGuardedFunction(std::weak_ptr<bool> flag, TokenizedFunction<TArgs...> function, unsigned int token)
        {
            if(function != nullptr)
            {
                return std::bind(_Internal::guardedFunction<TArgs...>, flag, function, token, std::placeholders::_1, std::placeholders::_2);
            }

            return nullptr;
        }
    };

    template <typename ... TArgs>
    struct Guarder<3, TArgs...>
    {
        static Function<TArgs...> createGuardedFunction(std::weak_ptr<bool> flag, TokenizedFunction<TArgs...> function, unsigned int token)
        {
            if(function != nullptr)
            {
                return std::bind(_Internal::guardedFunction<TArgs...>, flag, function, token, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            }

            return nullptr;
        }
    };

    template <typename ... TArgs>
    struct Guarder<4, TArgs...>
    {
        static Function<TArgs...> createGuardedFunction(std::weak_ptr<bool> flag, TokenizedFunction<TArgs...> function, unsigned int token)
        {
            if(function != nullptr)
            {
                return std::bind(_Internal::guardedFunction<TArgs...>, flag, function, token,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
            }

            return nullptr;
        }
    };

    template <typename ... TArgs>
    struct Guarder<5, TArgs...>
    {
        static Function<TArgs...> createGuardedFunction(std::weak_ptr<bool> flag, TokenizedFunction<TArgs...> function, unsigned int token)
        {
            if(function != nullptr)
            {
                return std::bind(_Internal::guardedFunction<TArgs...>, flag, function, token,
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
            }

            return nullptr;
        }
    };
}


template<typename ... TArgs>
class Guard
{
private:
    unsigned int token;
    std::shared_ptr<bool> existanceFlag;

public:
    typedef std::function<void(unsigned int token, const TArgs&...)> TokenizedFunction;
    typedef std::function<void(const TArgs&...)> Function;

    Guard() : token(0), existanceFlag(new bool(true)){}
    Guard(const Guard<TArgs...>&) : token(0), existanceFlag(new bool(true)) {}

    Function bind(TokenizedFunction tokenizedFunction)
    {
        if(tokenizedFunction != nullptr)
        {
            std::weak_ptr<bool> weakFlag(existanceFlag);

            return _Internal::Guarder<sizeof...(TArgs), TArgs...>::createGuardedFunction(weakFlag, tokenizedFunction, ++token);
        }
        
        return nullptr;
    }
};

