#ifndef CHAT_SERVER_HEAD_QEDFEPOBGHMNYG
#define CHAT_SERVER_HEAD_QEDFEPOBGHMNYG

#include <list>
#include <memory>

#include <ekutils/listener_socket_d.hpp>
#include <ekutils/epoll_d.hpp>

#include "connection.hpp"

namespace ktlo::chat {

typedef std::unique_ptr<ekutils::listener_socket_d> listener_ptr;

class server final {
	ekutils::epoll_d & epoll;
	listener_ptr sock;
	std::list<connection> connections;

public:
	explicit server(ekutils::epoll_d & poll);
	void broadcast(const std::string & username, const std::string & message);
	std::size_t there(const std::string & username) const;
	ekutils::epoll_d & poll() noexcept {
		return epoll;
	}

private:
	void on_accept();

};

} // namespace ktlo::chat

#endif // CHAT_SERVER_HEAD_QEDFEPOBGHMNYG
