#ifndef CHAT_GATE_HEAD_PQOVKTKBGFVFSK
#define CHAT_GATE_HEAD_PQOVKTKBGFVFSK

#include <memory>

#include <ekutils/expandbuff.hpp>
#include <ekutils/stream_socket_d.hpp>

namespace ktlo::chat {

typedef std::unique_ptr<ekutils::stream_socket_d> sock_ptr;

class bad_request : public std::runtime_error {
public:
	explicit bad_request(const std::string & message) : std::runtime_error(message) {}
	explicit bad_request(const char * message) : std::runtime_error(message) {}
};

class gate {
	sock_ptr sock;
	ekutils::expandbuff input, output;
public:
	explicit gate(sock_ptr && socket) : sock(std::move(socket)) {}
	bool head(std::int32_t & id, std::int32_t & size) const;
	template <typename P>
	bool paket_read(P & packet);
	template <typename P>
	void paket_write(const P & packet);
	void tunnel(gate & other);
	void receive();
	void send();
	std::size_t avail_read() const noexcept {
		return input.size();
	}
	std::size_t avail_write() const noexcept {
		return output.size();
	}
	ekutils::stream_socket_d & socket() noexcept {
		return *sock;
	}
};

template <typename P>
bool gate::paket_read(P & packet) {
	std::int32_t id, size;
	if (!head(id, size))
		return false;
	int s = packet.read(input.data(), size);
	if (std::int32_t(s) != size)
		throw bad_request("packet format error " + std::to_string(s) + " " + std::to_string(size));
	input.move(size);
	return true;
}

template <typename P>
void gate::paket_write(const P & packet) {
	std::size_t size = 10 + packet.size();
	output.asize(size);
	int s = packet.write(output.data() + output.size() - size, size);
#	ifdef DEBUG
		if (s < 0)
			throw "IMPOSSIBLE SITUATION";
#	endif
	output.ssize(size - s);
	send(); // init transmission
}

} // namespace ktlo::chat

#endif // CHAT_GATE_HEAD_PQOVKTKBGFVFSK
