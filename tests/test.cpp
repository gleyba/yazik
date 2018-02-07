#include <iostream>
#include <array>
#include <boost/context/all.hpp>
#include <future>

#define __l_move(x) x = std::move(x)

namespace yazik {

namespace executor {

class t {

};

} //end of yazik::executor namespace

namespace loop {

class t {

};

} //end of yazik::loop namespace

namespace async {



} //end of yazik::async

namespace coro {

namespace ctx = boost::context;

class t {

    ctx::continuation source_;


};

} //end of yazik::coro namespace

} //end of yazik namespace

int main() {
    namespace ctx = boost::context;
    ctx::continuation source=ctx::callcc
    (
        [](ctx::continuation && sink)
        {
            int a=0;
            int b=1;
            for(;;)
            {
                sink=sink.resume();
                auto next=a+b;
                a=b;
                b=next;
            }
            return std::move(sink);
        }
    );

    std::async(
      std::launch::async,
      [__l_move(source)] () mutable {
        for (int j=0;j<10;++j) {
            if (source) {
                source=source.resume();
                std::cout << source << std::endl;
            }
        }
      }
    ).get();

    return 0;
}
