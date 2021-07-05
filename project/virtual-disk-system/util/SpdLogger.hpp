


class SpdLogger
{
public:
	explicit SpdLogger()
	{
		auto max_size = 1048576 * 5;
		auto max_files = 3;
	}
	~SpdLogger();
	static std::shared_ptr<spdlog::logger> m_logger;
private:
};