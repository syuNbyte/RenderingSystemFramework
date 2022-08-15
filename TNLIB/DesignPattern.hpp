
// STL
#include<vector>
#include<unordered_map>

// TNLIB
#include"Debug.hpp"
#include"System.hpp"

namespace DesignPattern {

	namespace Observer {

	}

	// Fly-Weightパターン
	namespace FlyWeight {

		// 前方宣言
		template<class T>
		class Original;

		template<class T>
		class Copy;

		template<class T>
		class ManagedData;

		template<class T>
		class CManagedData;

		template<class T>
		class OriginalManager;

		// FlyWeightパターンの元データを持つクラス
		template<class T>
		class Original {
		public:
			friend OriginalManager;
		public:
			friend Copy;
		public:
			using original_type = T;
			using copy_type = Copy<T>;
		protected:
			T* m_instance;
			std::vector<Copy<T>*> m_copys;
		public:
			Original() {
				m_instance = new T;
			}
			Original(const Original& original) = delete;
			Original(Original&& original) {
				m_instance = original.m_instance;
				original.m_instance = nullptr;
				m_copys = original.m_copys;
				original.m_copys.clear();
				for (auto copy : m_copys) copy->m_original = this;
			};
			~Original() {
				if(m_instance != nullptr) delete m_instance;
				for (auto copy : m_copys) copy->m_original = nullptr;
			}
			Original& operator=(const Original& original) = delete;
			Original& operator=(Original&& original) {
				m_instance = original.m_instance;
				original.m_instance = nullptr;
				m_copys = original.m_copys;
				original.m_copys.clear();
				for (auto copy : m_copys) copy->m_original = this;
				return *this;
			}
		public:
			bool IsSafe() {
				return (m_instance != nullptr);
			}

			bool IsNull() {
				return (m_instance == nullptr);
			}

			std::size_t UseCount() {
				return m_copys.size();
			}

			T& Get() {
				return *m_instance;
			}

			T& SGet() {
				if (m_instance != nullptr) {
					return *m_instance;
				}
				else {
					Debug::LogError(
						"FlyWeight::Original<", typeid(T).name(), ">::SGet Method\n",
						"Instance is null."
					);
				}
			}

			Copy<T> CreateCopy() {
				return Copy<T>(*this);
			}
		};

		// FlyWeightパターンの元データをコピーして使用するクラス
		template<class T>
		class Copy {
		public:
			friend Original;
		protected:
			Original<T>* m_original;
		public:
			Copy() { m_original = nullptr; };
			Copy(const Copy& copy) {
				m_original = copy.m_original;
			}
			Copy(Original<T>& original) {
				m_original = &original;
				original.m_copys.push_back(this);
			}
			~Copy() {
				if (m_original != nullptr) {
					auto iter = std::find(m_original->m_copys.begin(), m_original->m_copys.end(), this);
					if(iter != m_original->m_copys.end()) m_original->m_copys.erase(iter);
				}
			}
			Copy& operator=(const Copy& copy) {
				m_original = copy.m_original;
				return *this;
			}
			Copy& operator=(Original<T>& original) {
				m_original = &original;
				original.m_copys.push_back(this);
				return *this;
			}
		public:
			bool IsSafe() {
				if (m_original != nullptr) {
					if (m_original->IsSafe()) {
						return true;
					}
					else {
						//Debug::Log("Has not ", typeid(T).name(), " Instance.");
						return false;
					}
				}
				else {
					//Debug::Log("Has not Original Instance.");
					return false;
				}
			}

			bool IsNull() {
				return (m_original->IsNull());
			}

			bool HasOriginal() {
				return (m_original != nullptr);
			}

			T& Get() {
				return m_original->Get();
			}

			T& SGet() {
				if (m_original->IsNull() != false) {
					return m_original->Get();
				}
				else {
					Debug::LogError(
						"FlyWeight::Copy<", typeid(T).name(), ">::Get Method\n",
						"FlyWeight::Original<", typeid(T).name(), ">::Get Method Instance Is Nullptr."
					);
				}
			}
		};

		// OriginalManagerクラスから取得したインスタンスの必要情報を保持するクラス
		template<class T>
		class ManagedData {
		private:
			std::string m_registID;
			T* m_instanceData;
		public:
			ManagedData() = default;
			ManagedData(const ManagedData& managedData) = default;
			ManagedData(ManagedData&& managedData) = default;
			ManagedData(const std::string& registID, T* instanceData) {
				m_registID = registID;
				m_instanceData = instanceData;
			}
		public:
			bool IsSafe() {
				return (m_instanceData != nullptr);
			}

			std::string GetRegistID() {
				return m_registID;
			}

			T& GetInstanceData() {
				return *m_instanceData;
			}
		};

		// ManagedDataクラスの定数版
		template<class T>
		class CManagedData {
		private:
			std::string m_registID;
			T* m_instanceData; // 元のデータのコピー
		public:
			CManagedData() = default;
			CManagedData(const CManagedData& managedData) = default;
			CManagedData(CManagedData&& managedData) = default;
			CManagedData(const std::string& registID, T* instanceData) {
				m_registID = registID;
				m_instanceData = instanceData;
			}
		public:
			bool IsSafe() {
				return (m_instanceData != nullptr);
			}

			std::string GetRegistID() {
				return m_registID;
			}

			T GetInstanceData() {
				return *m_instanceData;
			}
		};

		// FlyWeightパターンのオリジナルのデータを一元管理するマネージャークラス
		template<class T>
		class OriginalManager {
		private:
			std::unordered_map<std::string, Original<T>> m_originals;
		private:
			OriginalManager() {};
		private:
			static OriginalManager<T>& GetInstance() {
				static OriginalManager<T> originalPacker;
				return originalPacker;
			}
		public:
			// オリジナルデータの作成と登録を行う
			static CManagedData<T> Create() {
				Original<T> original;
				return OriginalManager<T>::Regist(std::move(original));
			}

			// 登録作業
			static CManagedData<T> Regist(Original<T>&& original) {
				OriginalManager<T>& instance = GetInstance();
				std::string id = System::GetGUIDStr();
				T* instanceData = original.m_instance;
				instance.m_originals.emplace(id, std::move(original));
				return CManagedData<T>(id, instanceData);
			}

			// 解放作業
			// 戻り値を受け取った場合はそのインスタンスは保持される
			// 戻り値を受け取らなかった場合は疎のインスタンスは破棄される
			static Original<T> Release(const std::string& registID) {
				OriginalManager<T>& instance = GetInstance();
				auto iter = instance.m_originals.find(registID);
				if (iter != instance.m_originals.cend()) {
					Original<T> original = std::move(iter->second);
					instance.m_originals.erase(iter);
					return std::move(original);
				};
			}

			// 検索して管理データを取得する
			static ManagedData<T> FindManagedData(const std::string& registID) {
				OriginalManager<T>& instance = GetInstance();
				auto iter = instance.m_originals.find(registID);
				if (iter != instance.m_originals.cend()) return ManagedData<T>(registID, iter->second.m_instance);
				else return ManagedData<T>(registID, nullptr);
			}

			// 検索してコピーを取得する
			static Copy<T> Find(const std::string& registID) {
				OriginalManager<T>& instance = GetInstance();
				auto iter = instance.m_originals.find(registID);
				if (iter != instance.m_originals.cend()) return iter->second;
				else return Copy<T>();
			}

			// 全ての登録IDを取得する
			static std::vector<std::string> GetRegistIDs() {
				std::vector<std::string> ret;
				OriginalManager<T>& instance = GetInstance();
				for (auto& original : instance.m_originals) ret.push_back(original.first);
				return ret;
			}

		};
	}



}