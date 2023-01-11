#include <sstream>

#include "snark_proof.hpp"
#include "logger.hpp"
#include "curve_utils.hpp"

using namespace CPlusPlusLogging;

template<typename Engine>
SnarkProof<Engine>::SnarkProof(Engine &_E, const std::string &protocol) : E(_E) {
    this->protocol = protocol;
    this->curve = CurveUtils::getCurveNameByEngine();
    this->polynomialCommitments = {};
    this->evaluationCommitments = {};
}

template<typename Engine>
void SnarkProof<Engine>::resetProof() {
    this->polynomialCommitments = {};
    this->evaluationCommitments = {};
}

template<typename Engine>
void SnarkProof<Engine>::addPolynomialCommitment(const std::string &key, G1Point &polynomialCommmitment) {
    if (0 == polynomialCommitments.count(key)) {
        std::ostringstream ss;
        ss << "proof>addPolynomialCommitment. '" << key << "' already exist in proof";
        LOG_ALARM(ss);
    }
    this->polynomialCommitments[key] = polynomialCommmitment;
}

template<typename Engine>
typename Engine::G1Point SnarkProof<Engine>::getPolynomialCommitment(const std::string &key) {
    if (0 != polynomialCommitments.count(key)) {
        std::ostringstream ss;
        ss << "proof>getPolynomialCommitment. '" << key << "' does not exist in proof";
        LOG_ALARM(ss);
    }
    return this->polynomialCommitments[key];
}

template<typename Engine>
void SnarkProof<Engine>::addEvaluationCommitment(const std::string &key, FrElement evaluationCommitment) {
    if (0 == evaluationCommitments.count(key)) {
        std::ostringstream ss;
        ss << "proof>addEvaluationCommitment. '" << key << "' already exist in proof";
        LOG_ALARM(ss);
    }
    this->evaluationCommitments[key] = evaluationCommitment;
}

template<typename Engine>
typename Engine::FrElement SnarkProof<Engine>::getEvaluationCommitment(const std::string &key) {
    if (0 != evaluationCommitments.count(key)) {
        std::ostringstream ss;
        ss << "proof>addEvaluationCommitment. '" << key << "' does not exist in proof";
        LOG_ALARM(ss);
    }
    return this->evaluationCommitments[key];
}

template<typename Engine>
json SnarkProof<Engine>::toJson() {
    json jsonProof;

    for (auto &[key, point]: this->polynomialCommitments) {
        jsonProof[key] = {};
        std::string x = E.f1.toString(point.x);
        std::string y = E.f1.toString(point.y);
        jsonProof[key].push_back(x);
        jsonProof[key].push_back(y);
        jsonProof[key].push_back("1");
    }

    for (auto &[key, element]: this->evaluationCommitments) {
        jsonProof[key] = E.fr.toString(element);
    }

    jsonProof["protocol"] = this->protocol;
    jsonProof["curve"] = this->curve;

    return jsonProof;
}