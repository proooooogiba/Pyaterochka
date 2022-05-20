#include <unistd.h>
#include <fcntl.h>

#include <thread>

#include "lemmatizator.h"


Lemmatizator::Lemmatizator() {

}
Lemmatizator::~Lemmatizator() {
    if (!initialized) {
        return;
    }
    if (stem_pid == 0) {
        close(to_stem[0]);
        close(from_stem[1]);
    } else {
        close(to_stem[1]);
        close(from_stem[0]);
    }
}
bool Lemmatizator::is_initialized() {
    return initialized;
}

bool Lemmatizator::initialize() {
    if (pipe(from_stem) != 0) {
        return false;
    }
    if (pipe(to_stem) != 0) {
        close(from_stem[0]);
        close(from_stem[1]);
        return false;
    }
    stem_pid = fork();
    if (stem_pid > 0) {
        // LOGGER << "running mystem process with pid: " << stem_pid << " from parent with pid: " << getpid() << "\n";
        // родительский процесс
        close(to_stem[0]);
        close(from_stem[1]);
        configFromStemOut();
        initialized = true;
    } else if (stem_pid == 0) {
        // LOGGER << "(child) my pid is: " << getpid() << "\n";
        // дочерний процесс
        dup2(to_stem[0], STDIN_FILENO);
        dup2(from_stem[1], STDOUT_FILENO);
        close(to_stem[1]);
        close(from_stem[0]);
        execv(current_lem.data(), (char *const *)stem_args.data());
        //TODO обработать ошибку запуска лемматизатора
        // write(error_pipe)...?
        // LOGGER << "Failed to run mystem app\n";
        exit(1);
    } else {
        // LOGGER << "Error while creating mystem process\n";
    }
    return initialized;
}

std::string Lemmatizator::lemmatize(const std::string& content) {
    if(!initialized) {
        // throw UninitializedLem();
        return std::string{};
    }
    std::string return_content{};
    return_content.resize(content.size() * 2);
    //TODO: обработать ошибки
    // ssize_t w = write(to_stem[1], content.data(), content.size());
    write(to_stem[1], content.data(), content.size());
    if (*content.end() != '\n') {
        char newline = '\n';
        write(to_stem[1], &newline, 1);
    }
    ssize_t total_read = 0;
    while (true) {
        ssize_t r = read(from_stem[0], return_content.data() + total_read, return_content.size() - total_read);
        //TODO обработать другие ошибки
        if (r == -1 && errno == EWOULDBLOCK) {
            using std::chrono_literals::operator""ms;
            std::this_thread::sleep_for(20ms);
            continue;
        }
        total_read += r;
        if (total_read > return_content.size() * 0.75) {
            return_content.resize(return_content.size() * 2);
        }
        if (std::count(return_content.begin(), return_content.end(), '[') ==
            std::count(return_content.begin(), return_content.end(), ']')) {
            break;
        }
    }
    // LOGGER << "content: <" << content.data() << ">  size=" << content.size() << "\n";
    // LOGGER << "sended: " << w << "   recived: " << total_read << "\n";
    // LOGGER << "recived content data: " << return_content.data() << "\n";
    // LOGGER << "recived content: " << return_content << "\n";
    // LOGGER << "recived content size: " << return_content.size() << "\n\n\n";
    return_content.resize(total_read);
    return_content.shrink_to_fit();

    return parseRecivedContent(return_content);
}
std::string Lemmatizator::parseRecivedContent(const std::string& content) {
    json json_content = json::parse(content);
    std::string return_content{};
    for (auto& analysis_obj : json_content) {
        if (analysis_obj["analysis"].size() != 0) {
            std::string lex = analysis_obj["analysis"][0]["lex"];
            return_content.append(lex);
        } else {
            std::string word = analysis_obj["text"];
            return_content.append(word);
        }
        return_content.push_back(' ');
    }
    if (*return_content.end() == ' ') {
        return_content.resize(return_content.size() - 1);
    }
    return return_content;
}
void Lemmatizator::configFromStemOut() {
    int flags = fcntl(from_stem[0], F_GETFL);
    fcntl(from_stem[0], F_SETFL, flags | O_NONBLOCK);
}
