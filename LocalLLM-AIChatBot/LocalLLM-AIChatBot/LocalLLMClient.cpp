#include "LocalLLMClient.h"

#include <sstream>
#include <string>
#include <windows.h>
#include <winhttp.h>

namespace
{
    // Ollama's default local address is http://localhost:11434.
    // Change these values when your Ollama address or model name changes.
    const wchar_t OLLAMA_HOST[] = L"localhost";
    const INTERNET_PORT OLLAMA_PORT = 11434;
    const wchar_t OLLAMA_GENERATE_PATH[] = L"/api/generate";
    const char OLLAMA_MODEL_NAME[] = "llama3.2";

    void closeHandle(HINTERNET handle)
    {
        if (handle != nullptr)
        {
            WinHttpCloseHandle(handle);
        }
    }

    std::string escapeJsonString(const std::string& text)
    {
        std::string escaped;

        for (char ch : text)
        {
            if (ch == '\\')
            {
                escaped += "\\\\";
            }
            else if (ch == '"')
            {
                escaped += "\\\"";
            }
            else if (ch == '\n')
            {
                escaped += "\\n";
            }
            else if (ch == '\r')
            {
                escaped += "\\r";
            }
            else if (ch == '\t')
            {
                escaped += "\\t";
            }
            else
            {
                escaped += ch;
            }
        }

        return escaped;
    }

    std::string createGenerateRequestBody(const std::string& prompt)
    {
        // Ollama /api/generate accepts JSON with model, prompt, and stream fields.
        std::ostringstream body;
        body << "{";
        body << "\"model\":\"" << OLLAMA_MODEL_NAME << "\",";
        body << "\"prompt\":\"" << escapeJsonString(prompt) << "\",";
        body << "\"stream\":false";
        body << "}";

        return body.str();
    }

    bool readHttpResponse(HINTERNET request, std::string& responseBody)
    {
        DWORD bytesAvailable = 0;

        do
        {
            if (!WinHttpQueryDataAvailable(request, &bytesAvailable))
            {
                return false;
            }

            if (bytesAvailable == 0)
            {
                break;
            }

            std::string buffer(bytesAvailable, '\0');
            DWORD bytesRead = 0;

            if (!WinHttpReadData(request, &buffer[0], bytesAvailable, &bytesRead))
            {
                return false;
            }

            buffer.resize(bytesRead);
            responseBody += buffer;
        } while (bytesAvailable > 0);

        return true;
    }

    bool isSuccessfulStatusCode(HINTERNET request)
    {
        DWORD statusCode = 0;
        DWORD statusCodeSize = sizeof(statusCode);

        if (!WinHttpQueryHeaders(
            request,
            WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX,
            &statusCode,
            &statusCodeSize,
            WINHTTP_NO_HEADER_INDEX))
        {
            return false;
        }

        return statusCode >= 200 && statusCode < 300;
    }

    bool extractJsonStringValue(const std::string& json, const std::string& fieldName, std::string& value)
    {
        // This small parser only reads the simple string field needed from Ollama: response.
        const std::string fieldText = "\"" + fieldName + "\"";
        const std::size_t fieldPosition = json.find(fieldText);

        if (fieldPosition == std::string::npos)
        {
            return false;
        }

        const std::size_t colonPosition = json.find(':', fieldPosition + fieldText.size());

        if (colonPosition == std::string::npos)
        {
            return false;
        }

        const std::size_t quotePosition = json.find('"', colonPosition + 1);

        if (quotePosition == std::string::npos)
        {
            return false;
        }

        value.clear();

        for (std::size_t index = quotePosition + 1; index < json.size(); ++index)
        {
            const char ch = json[index];

            if (ch == '"')
            {
                return true;
            }

            if (ch == '\\' && index + 1 < json.size())
            {
                ++index;
                const char escaped = json[index];

                if (escaped == 'n')
                {
                    value += '\n';
                }
                else if (escaped == 'r')
                {
                    value += '\r';
                }
                else if (escaped == 't')
                {
                    value += '\t';
                }
                else
                {
                    value += escaped;
                }
            }
            else
            {
                value += ch;
            }
        }

        return false;
    }
}

LocalLLMClient::LocalLLMClient() = default;

LocalLLMClient::~LocalLLMClient() = default;

bool LocalLLMClient::generateResponse(const std::string& prompt, std::string& response) const
{
    response.clear();

    HINTERNET session = WinHttpOpen(
        L"LocalLLM-AIChatBot/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    if (session == nullptr)
    {
        return false;
    }

    WinHttpSetTimeouts(session, 3000, 3000, 3000, 15000);

    HINTERNET connection = WinHttpConnect(session, OLLAMA_HOST, OLLAMA_PORT, 0);

    if (connection == nullptr)
    {
        closeHandle(session);
        return false;
    }

    HINTERNET request = WinHttpOpenRequest(
        connection,
        L"POST",
        OLLAMA_GENERATE_PATH,
        nullptr,
        WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        0);

    if (request == nullptr)
    {
        closeHandle(connection);
        closeHandle(session);
        return false;
    }

    const std::string requestBody = createGenerateRequestBody(prompt);
    const wchar_t headers[] = L"Content-Type: application/json\r\n";

    const BOOL sent = WinHttpSendRequest(
        request,
        headers,
        static_cast<DWORD>(-1L),
        const_cast<char*>(requestBody.data()),
        static_cast<DWORD>(requestBody.size()),
        static_cast<DWORD>(requestBody.size()),
        0);

    if (!sent || !WinHttpReceiveResponse(request, nullptr) || !isSuccessfulStatusCode(request))
    {
        closeHandle(request);
        closeHandle(connection);
        closeHandle(session);
        return false;
    }

    std::string responseBody;
    const bool readResponse = readHttpResponse(request, responseBody);

    closeHandle(request);
    closeHandle(connection);
    closeHandle(session);

    if (!readResponse)
    {
        return false;
    }

    return extractJsonStringValue(responseBody, "response", response) && !response.empty();
}
