/*
 * clarity_reporter.hpp — reporter Catch2 personnalisé "clarity"
 * -----------------------------------------------------------------------
 * But : donner aux étudiants des messages de succès/échec clairs,
 * lisibles et colorés, en s'appuyant sur le vrai moteur de test de
 * Catch2 (v2.13.4) — TEST_CASE, REQUIRE, CHECK, SECTION, etc.
 * fonctionnent exactement comme d'habitude. Seul l'affichage change.
 *
 * Couleurs : on écrit nous-mêmes des codes ANSI (plutôt que d'utiliser
 * Catch::Colour, qui sur Windows appelle SetConsoleTextAttribute — une
 * API qui ne fonctionne pas quand la sortie est capturée par un pipe,
 * ce que font la plupart des IDE, dont CLion). Au démarrage, sur
 * Windows, on active en plus le support ANSI natif de la console
 * (ENABLE_VIRTUAL_TERMINAL_PROCESSING) pour que ça fonctionne aussi
 * dans un vrai cmd.exe / PowerShell, pas seulement dans CLion.
 *
 * Utilisation :
 *
 *   1) Dans UN SEUL fichier .cpp du projet :
 *
 *        #define CATCH_CONFIG_NO_POSIX_SIGNALS  // voir remarque plus bas
 *        #define CATCH_CONFIG_MAIN
 *        #define CATCH_CONFIG_DEFAULT_REPORTER "clarity"
 *        #include "catch.hpp"
 *        #include "clarity_reporter.hpp"
 *
 *   2) Dans vos fichiers de test :
 *
 *        #include "catch.hpp"
 *
 *        TEST_CASE("addition simple") {
 *            REQUIRE(2 + 2 == 4);
 *            CHECK(2 + 3 == 5);
 *        }
 *
 *   3) Compilation :
 *        g++ -std=c++14 main_tests.cpp -o tests
 *      (catch.hpp étant un header-only, aucune autre dépendance)
 *
 *   On peut aussi choisir le reporter en ligne de commande, sans rien
 *   changer au code, avec :  ./tests -r clarity
 *   Pour désactiver explicitement la couleur :  ./tests -r clarity --use-colour no
 *
 *   Remarque (glibc récente / CLion) : catch.hpp 2.13.4 ne compile pas
 *   tel quel sur les systèmes où MINSIGSTKSZ n'est plus une constante
 *   de compilation (glibc >= 2.34, ou toolchains récentes type MinGW).
 *   Définir CATCH_CONFIG_NO_POSIX_SIGNALS avant d'inclure catch.hpp
 *   règle le problème ; REQUIRE/CHECK ne sont pas affectés, seule la
 *   détection des crashs (segfault) façon "fatal error handler" est
 *   désactivée.
 * -----------------------------------------------------------------------
 */
#ifndef CLARITY_REPORTER_HPP_INCLUDED
#define CLARITY_REPORTER_HPP_INCLUDED

#include "catch.hpp"

#include <iostream>
#include <string>

#if defined(_WIN32)
//#  define NOMINMAX
#  include <windows.h>
#endif

namespace ClarityReporterDetail {

struct Colours {
    static constexpr const char* reset  = "\033[0m";
    static constexpr const char* green  = "\033[32m";
    static constexpr const char* red    = "\033[31m";
    static constexpr const char* bold   = "\033[1m";
    static constexpr const char* dim    = "\033[2m";
};

inline std::string plural(std::size_t n, const char* singular) {
    return std::to_string(n) + " " + singular + (n > 1 ? "s" : "");
}

// Active l'interprétation des codes ANSI dans une vraie console Windows
// (cmd.exe / PowerShell). Sans effet ailleurs (Linux/macOS, ou quand la
// sortie est déjà capturée par un pipe comme dans CLion — cela ne fait
// alors simplement rien, sans erreur).
inline void enableAnsiOnWindowsConsole() {
#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return; // pas une vraie console (pipe) : rien à faire
    SetConsoleMode(hOut, mode | 0x0004 /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */);
#endif
}

} // namespace ClarityReporterDetail

class ClarityReporter : public Catch::StreamingReporterBase<ClarityReporter> {
public:
    ClarityReporter(Catch::ReporterConfig const& config)
        : StreamingReporterBase(config) {
        // On veut voir passer TOUTES les assertions (pas seulement les
        // échecs) pour pouvoir donner un retour complet.
        m_reporterPrefs.shouldReportAllAssertions = true;
        ClarityReporterDetail::enableAnsiOnWindowsConsole();
        m_useColour = (m_config->useColour() != Catch::UseColour::No);
    }

    ~ClarityReporter() override = default;

    static std::string getDescription() {
        return "Messages de succes/echec clairs et colores, pour un usage pedagogique";
    }

    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override {
        StreamingReporterBase::testCaseStarting(testInfo);
        m_checksInTest = 0;
        m_failuresInTest = 0;
        stream << "\n" << c(ClarityReporterDetail::Colours::bold)
               << "TEST : " << testInfo.name << c(ClarityReporterDetail::Colours::reset)
               << "\n";
    }

    void assertionStarting(Catch::AssertionInfo const&) override {}

    bool assertionEnded(Catch::AssertionStats const& stats) override {
        Catch::AssertionResult const& result = stats.assertionResult;

        if (result.getResultType() == Catch::ResultWas::Warning) {
            return true;
        }

        ++m_checksInTest;
        bool passed = result.isOk();
        if (!passed) {
            ++m_failuresInTest;
        }

        std::string expr = result.hasExpression() ? result.getExpression() : std::string();

        if (passed) {
            stream << "    " << c(ClarityReporterDetail::Colours::green) << "[OK]  "
                   << c(ClarityReporterDetail::Colours::reset) << expr << "\n";
        } else {
            stream << "    " << c(ClarityReporterDetail::Colours::red) << "[FAIL]"
                   << c(ClarityReporterDetail::Colours::reset) << " " << expr << "\n";

            if (result.hasExpandedExpression() &&
                result.getExpandedExpression() != expr) {
                stream << "      " << c(ClarityReporterDetail::Colours::dim)
                       << "-> valeur obtenue : " << result.getExpandedExpression()
                       << c(ClarityReporterDetail::Colours::reset) << "\n";
            }
            if (result.hasMessage()) {
                stream << "      " << c(ClarityReporterDetail::Colours::dim)
                       << "-> message : " << result.getMessage()
                       << c(ClarityReporterDetail::Colours::reset) << "\n";
            }
            stream << "      " << c(ClarityReporterDetail::Colours::dim)
                   << "-> " << result.getSourceInfo().file << ":"
                   << result.getSourceInfo().line
                   << c(ClarityReporterDetail::Colours::reset) << "\n";
        }

        for (auto const& msg : stats.infoMessages) {
            if (msg.type == Catch::ResultWas::Info) {
                stream << "      " << c(ClarityReporterDetail::Colours::dim)
                       << "info : " << msg.message
                       << c(ClarityReporterDetail::Colours::reset) << "\n";
            }
        }

        return true;
    }

    void testCaseEnded(Catch::TestCaseStats const& testCaseStats) override {
        if (m_failuresInTest == 0) {
            stream << "  " << c(ClarityReporterDetail::Colours::green) << "OK "
                   << testCaseStats.testInfo.name << " -- "
                   << ClarityReporterDetail::plural(m_checksInTest, "verification")
                   << " reussie(s)" << c(ClarityReporterDetail::Colours::reset) << "\n";
        } else {
            stream << "  " << c(ClarityReporterDetail::Colours::red) << "ECHEC "
                   << testCaseStats.testInfo.name << " -- "
                   << ClarityReporterDetail::plural(m_failuresInTest, "echec")
                   << " sur "
                   << ClarityReporterDetail::plural(m_checksInTest, "verification")
                   << c(ClarityReporterDetail::Colours::reset) << "\n";
        }
        StreamingReporterBase::testCaseEnded(testCaseStats);
    }

    void testRunEnded(Catch::TestRunStats const& runStats) override {
        auto const& totals = runStats.totals;

        stream << "\n" << c(ClarityReporterDetail::Colours::bold)
               << "===============================================================================\n";

        if (totals.assertions.failed == 0) {
            stream << c(ClarityReporterDetail::Colours::green)
                   << "Tous les tests sont passes ("
                   << ClarityReporterDetail::plural(totals.assertions.total(), "verification")
                   << " dans "
                   << ClarityReporterDetail::plural(totals.testCases.total(), "test")
                   << ")" << c(ClarityReporterDetail::Colours::reset) << "\n";
        } else {
            stream << c(ClarityReporterDetail::Colours::red)
                   << "Des tests ont echoue" << c(ClarityReporterDetail::Colours::reset) << "\n";
            stream << "  tests        : " << totals.testCases.total()
                   << " | " << totals.testCases.passed << " reussi(s)"
                   << " | " << c(ClarityReporterDetail::Colours::red)
                   << totals.testCases.failed << " echoue(s)"
                   << c(ClarityReporterDetail::Colours::reset) << "\n";
            stream << "  verifications: " << totals.assertions.total()
                   << " | " << totals.assertions.passed << " reussie(s)"
                   << " | " << c(ClarityReporterDetail::Colours::red)
                   << totals.assertions.failed << " echouee(s)"
                   << c(ClarityReporterDetail::Colours::reset) << "\n";
        }
        stream << c(ClarityReporterDetail::Colours::bold)
               << "===============================================================================\n"
               << c(ClarityReporterDetail::Colours::reset);

        StreamingReporterBase::testRunEnded(runStats);
    }

private:
    const char* c(const char* code) const { return m_useColour ? code : ""; }

    bool m_useColour = true;
    std::size_t m_checksInTest = 0;
    std::size_t m_failuresInTest = 0;
};

CATCH_REGISTER_REPORTER("clarity", ClarityReporter)

#endif // CLARITY_REPORTER_HPP_INCLUDED