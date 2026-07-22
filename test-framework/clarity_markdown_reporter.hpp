/*
 * clarity_markdown_reporter.hpp — génère un rapport Markdown des tests
 * -----------------------------------------------------------------------
 * Ne remplace rien : c'est un LISTENER Catch2, qui s'ajoute à côté du
 * reporter choisi (par ex. "clarity") sans toucher à l'affichage
 * console existant. Il écoute simplement le déroulement des tests et,
 * à la fin, écrit un fichier Markdown récapitulatif dans le
 * répertoire "output" (créé automatiquement s'il n'existe pas, relatif
 * au répertoire de travail courant du programme).
 *
 * Utilisation : dans le fichier où se trouve déjà
 *   #define CATCH_CONFIG_MAIN
 *   ...
 *   #include "catch.hpp"
 *   #include "clarity_reporter.hpp"          // déjà présent
 *   #include "clarity_markdown_reporter.hpp" // à ajouter
 *
 * Rien d'autre à changer : le listener s'enregistre tout seul et
 * tourne automatiquement à chaque exécution, quel que soit le
 * reporter actif (-r clarity, -r console, etc.).
 * -----------------------------------------------------------------------
 */
#ifndef CLARITY_MARKDOWN_REPORTER_HPP_INCLUDED
#define CLARITY_MARKDOWN_REPORTER_HPP_INCLUDED

#include "catch.hpp"

#include <cctype>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#if defined(_WIN32)
#  include <direct.h>
#else
#  include <sys/stat.h>
#endif

namespace ClarityMarkdownDetail {

inline void ensureOutputDirExists(const std::string& dir) {
#if defined(_WIN32)
    _mkdir(dir.c_str());
#else
    mkdir(dir.c_str(), 0755);
#endif
}

inline std::string currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tmBuf{};
#if defined(_WIN32)
    localtime_s(&tmBuf, &t);
#else
    localtime_r(&t, &tmBuf);
#endif
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y ,%B %d : %H:%M", &tmBuf);
    return std::string(buf);
}

// Echappe les caractères qui casseraient une cellule de tableau Markdown.
inline std::string escapeForTable(std::string s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        if (ch == '|') out += "\\|";
        else if (ch == '\n') out += "<br>";
        else out += ch;
    }
    return out;
}

// Enlève les espaces ET les retours à la ligne en début/fin de chaîne.
inline std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Cherche un "==" de comparaison (entouré d'espace ou de saut de
// ligne, jamais collé à un caractère). Catch2 place parfois le "=="
// seul sur sa propre ligne quand les deux valeurs comparées sont
// multi-lignes, donc on ne peut pas se contenter de chercher " == ".
inline std::size_t findComparisonOperator(const std::string& s) {
    for (std::size_t i = 0; i + 1 < s.size(); ++i) {
        if (s[i] == '=' && s[i + 1] == '=') {
            bool beforeOk = (i == 0) || std::isspace(static_cast<unsigned char>(s[i - 1]));
            bool afterOk = (i + 2 >= s.size()) || std::isspace(static_cast<unsigned char>(s[i + 2]));
            if (beforeOk && afterOk) return i;
        }
    }
    return std::string::npos;
}

// Enlève une paire de guillemets qui engloberait toute la chaîne
// (ex: "\"bonjour\"" -> "bonjour"). Ne touche pas aux guillemets
// internes.
inline std::string stripWrappingQuotes(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

// Met en forme un texte pour une cellule de tableau Markdown : échappe
// les '|', et transforme en <br> aussi bien les vrais retours à la
// ligne (caractère '\n' réel, présent dans une valeur obtenue à
// l'exécution) que les séquences écrites en toutes lettres "\n"
// (telles qu'elles apparaissent dans le code source d'un test, donc
// littéralement un antislash suivi d'un 'n').
inline std::string formatMultilineForTable(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (ch == '|') {
            out += "\\|";
        } else if (ch == '\n') {
            out += "<br>";
        } else if (ch == '\\' && i + 1 < s.size() && s[i + 1] == 'n') {
            out += "<br>";
            ++i; // on a consommé le 'n' en plus de l'antislash
        } else {
            out += ch;
        }
    }
    return out;
}

// Colonne "Attendu" : à partir de l'expression source telle qu'écrite
// dans le test (ex: `output == "texte attendu"`), ne garde que ce qui
// suit le `==` (la valeur attendue), enlève les guillemets englobants
// et met en forme pour affichage Markdown. Si l'expression ne contient
// pas de `==` (ex: une simple condition booléenne), elle est affichée
// telle quelle.
inline std::string formatExpectedCell(const std::string& rawExpression) {
    std::string value = rawExpression;
    auto pos = findComparisonOperator(rawExpression);
    if (pos != std::string::npos) {
        value = rawExpression.substr(pos + 2);
    }
    value = trim(value);
    value = stripWrappingQuotes(value);
    return formatMultilineForTable(value);
}

// Colonne "Obtenu" : à partir de l'expression développée par Catch2 à
// l'exécution (de la forme `"valeur obtenue" == "valeur attendue"`),
// ne garde que la partie gauche (la valeur réellement obtenue), enlève
// les guillemets englobants et met en forme pour affichage Markdown.
inline std::string formatObtainedCell(const std::string& expandedExpression) {
    std::string value = expandedExpression;
    auto pos = findComparisonOperator(expandedExpression);
    if (pos != std::string::npos) {
        value = expandedExpression.substr(0, pos);
    }
    value = trim(value);
    value = stripWrappingQuotes(value);
    return formatMultilineForTable(value);
}
// Extrait les lignes encadrées par BEGIN_TEST_CODE et END_TEST_CODE
// autour de la ligne où l'assertion a échoué.
// Extrait les lignes encadrées par BEGIN_TEST_CODE et END_TEST_CODE
// autour de la ligne où l'assertion a échoué.
// Extrait proprement les lignes encadrées par BEGIN_TEST_CODE et END_TEST_CODE
// situées STRICTEMENT dans le TEST_CASE de l'erreur.
inline std::string extractTestCode(const std::string& filename, std::size_t errorLine) {
    std::ifstream file(filename);
    if (!file.is_open()) return "// Fichier source introuvable à l'exécution";

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    if (errorLine == 0 || errorLine > lines.size()) return "// Ligne introuvable";

    long errIdx = static_cast<long>(errorLine - 1);

    // Délimitation du TEST_CASE courant
    long testCaseStart = 0;
    for (long i = errIdx; i >= 0; --i) {
        if (lines[i].find("TEST_CASE") != std::string::npos) {
            testCaseStart = i;
            break;
        }
    }

    long testCaseEnd = static_cast<long>(lines.size());
    for (long i = errIdx + 1; i < static_cast<long>(lines.size()); ++i) {
        if (lines[i].find("TEST_CASE") != std::string::npos) {
            testCaseEnd = i;
            break;
        }
    }

    // Recherche des macros
    long startIdx = -1;
    long endIdx = -1;
    for (long i = testCaseStart; i < testCaseEnd; ++i) {
        if (lines[i].find("BEGIN_TEST_CODE") != std::string::npos && startIdx == -1) {
            startIdx = i + 1;
        } else if (lines[i].find("END_TEST_CODE") != std::string::npos && startIdx != -1) {
            endIdx = i;
            break;
        }
    }

    if (startIdx != -1 && endIdx != -1 && startIdx < endIdx) {
        std::string rawCode;
        for (long i = startIdx; i < endIdx; ++i) {
            rawCode += lines[i] + "\n";
        }
        return rawCode;
    }

    return "// Macros BEGIN_TEST_CODE / END_TEST_CODE introuvables";
}

struct FailureDetail {
    std::string expression;
    std::string expandedExpression;
    std::string message;
    std::string location; // fichier:ligne
	std::string testCode; // code de test à afficher aux étudiants
};

struct TestResult {
    std::string name;
    std::size_t checks = 0;
    std::size_t failures = 0;
    std::vector<FailureDetail> failureDetails;
    bool passed() const { return failures == 0; }
};

} // namespace ClarityMarkdownDetail

class ClarityMarkdownReporter : public Catch::TestEventListenerBase {
public:
    using TestEventListenerBase::TestEventListenerBase;

    void testRunStarting(Catch::TestRunInfo const& runInfo) override {
        TestEventListenerBase::testRunStarting(runInfo);
        m_results.clear();
        m_programName = m_config->name();
        if (m_programName.empty()) {
            m_programName = runInfo.name;
        }
    }

    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override {
        TestEventListenerBase::testCaseStarting(testInfo);
        m_results.emplace_back();
        m_results.back().name = testInfo.name;
    }

    bool assertionEnded(Catch::AssertionStats const& stats) override {
        Catch::AssertionResult const& result = stats.assertionResult;

        if (result.getResultType() == Catch::ResultWas::Warning || m_results.empty()) {
            return true;
        }

        auto& current = m_results.back();
        ++current.checks;

        if (!result.isOk()) {
            ++current.failures;

            ClarityMarkdownDetail::FailureDetail detail;
            detail.expression = result.hasExpression() ? result.getExpression() : std::string();
            if (result.hasExpandedExpression() &&
                result.getExpandedExpression() != detail.expression) {
                detail.expandedExpression = result.getExpandedExpression();
            }
            if (result.hasMessage()) {
                detail.message = result.getMessage();
            }
            std::ostringstream loc;
            loc << result.getSourceInfo().file << ":" << result.getSourceInfo().line;
            detail.location = loc.str();
			// --- reprise du code de test ---
            detail.testCode = ClarityMarkdownDetail::extractTestCode(
                result.getSourceInfo().file, 
                result.getSourceInfo().line
            );
            // -------------
            current.failureDetails.push_back(std::move(detail));
        }

        return true;
    }

    void testRunEnded(Catch::TestRunStats const& runStats) override {
        writeMarkdownReport(runStats);
        TestEventListenerBase::testRunEnded(runStats);
    }

private:
    void writeMarkdownReport(Catch::TestRunStats const& runStats) const {
        using namespace ClarityMarkdownDetail;

		#ifdef CLARITY_OUTPUT_DIR
			const std::string outputDir = CLARITY_OUTPUT_DIR;
		#else
			const std::string outputDir = "output";
		#endif
		
        ensureOutputDirExists(outputDir);

        std::string path = outputDir + "/rapport_tests.md";
        std::ofstream out(path, std::ios::out | std::ios::trunc);
        if (!out) {
            return; // on n'interrompt jamais les tests pour un probleme d'ecriture de rapport
        }

        auto const& totals = runStats.totals;
        bool allPassed = totals.assertions.failed == 0;

        out << "# 🧪 TEST REPORT for : " << m_programName << "\n\n";
        out << "*Generated on : " << currentTimestamp() << "*\n\n";

        // --- Résumé (badges shields.io + tableau global) -----------------
        out << "## 📊 Global result\n\n";

        std::string statusBadge = allPassed
            ? "![Status](https://img.shields.io/badge/status-SUCCESS-brightgreen)"
            : "![Statut](https://img.shields.io/badge/status-FAILED-orange)";
        out << statusBadge << " "
            << "![Tests](https://img.shields.io/badge/tests-"
            << totals.testCases.total() << "-blue) "
            << "![PASSED](https://img.shields.io/badge/passed-"
            << totals.testCases.passed << "-brightgreen) "
            << "![FAILED](https://img.shields.io/badge/failed-"
            << totals.testCases.failed << "-red)\n\n";

        out << "| " << " " << " | Total | ✅ PASSED | ❌ FAILED |\n";
        out << "|---|---:|---:|---:|\n";
        out << "| **Tests** | " << totals.testCases.total()
            << " | " << totals.testCases.passed
            << " | " << totals.testCases.failed << " |\n";
        out << "| **Checked** | " << totals.assertions.total()
            << " | " << totals.assertions.passed
            << " | " << totals.assertions.failed << " |\n\n";

        if (allPassed) {
            out << "> ✅ **ALL TEST PASSED.** Congratulations !\n\n";
        } else {
            out << "> ❌ **" << totals.testCases.failed
                << (totals.testCases.failed > 1 ? " tests are" : " test is")
                << " FAILED.** See details below.\n\n";
        }

        // --- Tableau détaillé par test ------------------------------------
        out << "## 📋 Checked tests\n\n";
        out << "| Status | Test | Checks done | PASSED | FAILED |\n";
        out << "|:---:|---|---:|---:|---:|\n";
        for (auto const& r : m_results) {
            std::size_t passedChecks = r.checks - r.failures;
            out << "| " << (r.passed() ? "✅" : "❌")
                << " | " << escapeForTable(r.name)
                << " | " << r.checks
                << " | " << passedChecks
                << " | " << r.failures << " |\n";
        }
        out << "\n";

        // --- Détail des échecs ---------------------------------------------
		if (!allPassed)
		{
            out << "## 🔎 FAILED tests for the moment\n\n";
            for (auto const& r : m_results)
			{
                if (r.failureDetails.empty()) continue;

                out << "### ❌ " << escapeForTable(r.name) << "\n\n";
                
                // Tableau sans la colonne 'Code testé'
                out << "| Expected | Got | Message |\n";
                out << "|---|---|---|\n";
                for (auto const& d : r.failureDetails)
				{
                    out << "| " << formatExpectedCell(d.expression)
                        << " | " << (d.expandedExpression.empty() ? "*—*" : formatObtainedCell(d.expandedExpression))
                        << " | " << (d.message.empty() ? "*—*" : formatMultilineForTable(d.message))
                        << " |\n";
                }
                out << "\n";

                // Bloc de code C++ en Markdown pur sous le tableau
                out << "**what is tested :**\n\n";
                out << "```cpp\n";
                for (auto const& d : r.failureDetails)
				{
                    out << d.testCode; // d.testCode contient déjà les retours à la ligne
                }
                out << "```\n\n";
            }
        }

        out << "---\n";
        out << "*Report completed. 🄯 N. Flasque 2026*\n";
    }

    std::vector<ClarityMarkdownDetail::TestResult> m_results;
    std::string m_programName;
};

CATCH_REGISTER_LISTENER(ClarityMarkdownReporter)

#endif // CLARITY_MARKDOWN_REPORTER_HPP_INCLUDED