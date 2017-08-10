/*
 * Copyright (C) 1998-2017 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <alps/mc/stop_callback.hpp>

#include <alps/utilities/signal.hpp>
#include <alps/utilities/boost_mpi.hpp>

namespace alps {

    stop_callback::stop_callback(std::size_t timelimit)
        : limit(timelimit)
        , start(std::time(0))
    {}

#ifdef ALPS_HAVE_MPI
    stop_callback::stop_callback(alps::mpi::communicator const & cm, std::size_t timelimit)
        : limit(timelimit), start(std::time(0)), comm(cm)
    {}
#endif

    bool stop_callback::operator()() const {
        std::time_t now(std::time(0));
#ifdef ALPS_HAVE_MPI
        if (comm) {
            bool to_stop;
            if (comm->rank() == 0)
                to_stop = !signals.empty() || (limit > 0 && std::difftime(now, start) > limit);
            broadcast(*comm, to_stop, 0);
            return to_stop;
        } else
#endif
            return !signals.empty() || (limit > 0 && std::difftime(now, start) > limit);
    }


    simple_time_callback::simple_time_callback(std::size_t timelimit)
        : limit(timelimit)
        , start(std::time(0))
    {}

    bool simple_time_callback::operator()() const {
        std::time_t now(time(0));
        return (limit > 0 && std::difftime(now, start) > limit);
    }
}
