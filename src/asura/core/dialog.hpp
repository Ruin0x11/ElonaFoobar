namespace dialog
{

enum class response : int
{
  ok = 1,
  yes = 6,
  no = 7,
};

enum class message_type
{
  information,
  warning,
  error
};

response ok(const std::string&, message_type = message_type::information);
response yes_or_no(const std::string&, message_type = message_type::information);

}
