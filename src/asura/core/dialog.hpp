namespace dialog
{

struct state
{
    state(std::string message) : message(message) {}
    state(std::string message, std::string submessage)
        : message(message)
        , submessage(submessage) {}

    std::string message;
    std::string submessage;
};

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

response ok(dialog::state&, message_type = message_type::information);
response yes_or_no(dialog::state&, message_type = message_type::information);

}
