#ifndef ERROR_LOG_HPP
# define ERROR_LOG_HPP

# include "Irc.hpp"

class errorLog {
	private:
		errorLog(errorLog const &src);
		errorLog &operator=(errorLog const &rhs);

	public:
		~errorLog();
		errorLog();
		bool ErrorLog(std::string error);
};

#endif
