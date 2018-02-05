#include <iostream>
#include <array>
#include <boost/context/all.hpp>
#include <future>

#define __l_move(x) x = std::move(x)

namespace ctx = boost::context;

int main() {
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
            if (source)
                source=source.resume();
        }
      }
    ).get();

    return 0;
}
